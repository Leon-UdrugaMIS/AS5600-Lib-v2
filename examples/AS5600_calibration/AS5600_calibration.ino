#include <Wire.h>
#include <AS5600.h>

AS5600 senzor;
AS5600Calibrator kalibracija;

const uint8_t PIN_TIPKALO = A0;

const unsigned long DEBOUNCE_MS = 25;
const unsigned long DUGI_PRITISAK_MS = 1000;
const unsigned long STARTNI_PROZOR_MS = 2000;
const unsigned long PERIOD_ISPISA_MS = 100;

enum StanjeRada {
  STARTNI_PROZOR,
  KALIBRACIJA_NULA,
  KALIBRACIJA_180,
  NORMALAN_RAD
};

StanjeRada stanje = STARTNI_PROZOR;

bool prethodnoStanjeTipkala = HIGH;
unsigned long vrijemePromjeneTipkala = 0;
unsigned long vrijemePritiskaTipkala = 0;
bool dugiPritisakObraden = false;

unsigned long vrijemePokretanja = 0;
unsigned long zadnjiIspis = 0;

uint16_t ocitajSirovuVrijednost()
{
  return senzor.rawAngle() & AS5600_RAW_MASK;
}

uint8_t ocitajDogadajTipkala()
{
  bool stanjeTipkala = digitalRead(PIN_TIPKALO);
  unsigned long sada = millis();

  if (stanjeTipkala != prethodnoStanjeTipkala)
  {
    vrijemePromjeneTipkala = sada;
    prethodnoStanjeTipkala = stanjeTipkala;
  }

  if ((sada - vrijemePromjeneTipkala) <= DEBOUNCE_MS)
  {
    return 0;
  }

  static bool biloPritisnuto = false;
  bool pritisnuto = (stanjeTipkala == LOW);

  if (pritisnuto && !biloPritisnuto)
  {
    vrijemePritiskaTipkala = sada;
    dugiPritisakObraden = false;
  }

  if (pritisnuto && !dugiPritisakObraden && (sada - vrijemePritiskaTipkala >= DUGI_PRITISAK_MS))
  {
    dugiPritisakObraden = true;
    biloPritisnuto = pritisnuto;
    return 2;
  }

  if (!pritisnuto && biloPritisnuto)
  {
    unsigned long trajanje = sada - vrijemePritiskaTipkala;
    if (trajanje >= DEBOUNCE_MS && trajanje < DUGI_PRITISAK_MS)
    {
      biloPritisnuto = pritisnuto;
      return 1;
    }
  }

  biloPritisnuto = pritisnuto;
  return 0;
}

void obradiStanja()
{
  uint8_t dogadaj = ocitajDogadajTipkala();
  unsigned long sada = millis();

  switch (stanje)
  {
    case STARTNI_PROZOR:
      if (dogadaj == 1 || dogadaj == 2)
      {
        stanje = KALIBRACIJA_NULA;
        Serial.println("Ulazak u kalibraciju.");
        Serial.println("Postavi krak na 0° i pritisni tipkalo.");
      }
      else if ((sada - vrijemePokretanja) >= STARTNI_PROZOR_MS)
      {
        stanje = NORMALAN_RAD;
        Serial.println("Normalan rad.");
      }
      break;

    case KALIBRACIJA_NULA:
      if (dogadaj == 1)
      {
        kalibracija.setCalibrationMin(ocitajSirovuVrijednost());
        Serial.print("Spremljeno 0°: ");
        Serial.println(kalibracija.getCalibrationMin());
        stanje = KALIBRACIJA_180;
        Serial.println("Postavi krak na 180° i pritisni tipkalo.");
      }
      break;

    case KALIBRACIJA_180:
      if (dogadaj == 1)
      {
        kalibracija.setCalibrationMax(ocitajSirovuVrijednost());
        kalibracija.setWorkingZeroCentiDegrees(0);
        kalibracija.saveToEEPROM();
        Serial.print("Spremljeno 180°: ");
        Serial.println(kalibracija.getCalibrationMax());
        Serial.println("Kalibracija završena.");
        stanje = NORMALAN_RAD;
      }
      break;

    case NORMALAN_RAD:
      if (dogadaj == 1)
      {
        uint16_t raw = ocitajSirovuVrijednost();
        kalibracija.setWorkingZeroCentiDegrees(kalibracija.computeCalibratedAngleCentiDegrees(raw));
        kalibracija.saveToEEPROM();
        Serial.println("Postavljena nova radna nula.");
      }
      break;
  }
}

void setup()
{
  pinMode(PIN_TIPKALO, INPUT_PULLUP);
  Wire.begin();
  senzor.begin();
  Serial.begin(9600);
  delay(300);

  kalibracija.loadFromEEPROM();

  vrijemePokretanja = millis();
  Serial.println("Kutomjer pokrenut.");
  Serial.println("U prve 2 sekunde moguc je ulazak u kalibraciju.");
  Serial.println("Kratki pritisak u normalnom radu postavlja radnu nulu.");
}

void loop()
{
  obradiStanja();

  unsigned long sada = millis();
  if ((sada - zadnjiIspis) >= PERIOD_ISPISA_MS)
  {
    zadnjiIspis = sada;
    uint16_t raw = ocitajSirovuVrijednost();

    if (stanje == KALIBRACIJA_NULA)
    {
      Serial.println("Kalibracija: cekam spremanje 0°");
    }
    else if (stanje == KALIBRACIJA_180)
    {
      Serial.println("Kalibracija: cekam spremanje 180°");
    }
    else
    {
      uint16_t kalibriraniKut = kalibracija.computeCalibratedAngleCentiDegrees(raw);
      uint16_t kutZaPrikaz = kalibracija.computeDisplayAngleCentiDegrees(raw);

      Serial.print("Ocitana sirova vrijednost kuta = ");
      Serial.println(raw);
      Serial.print("Preracunata vrijednost u stupnjevima = ");
      Serial.println(kutZaPrikaz / 100.0, 2);
      Serial.print("Kalibrirani kut = ");
      Serial.println(kalibriraniKut / 100.0, 2);
      Serial.println();
    }
  }
}
