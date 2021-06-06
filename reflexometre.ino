// Include du programme
#include <LiquidCrystal.h>

// Define du programme
#define LEDGREEN 13
#define LEDRED 12
#define BTNSTART 11
#define BTNACT 10

// Initialisation du LCD
LiquidCrystal lcd(2,3,4,5,6,7);

// Variable globale
char state;
unsigned long randNumber; // test console
unsigned long time1; // Temps led
unsigned long time2; // Temps btn
float timeTotal = 0; // Intervalle de temps
boolean etatBouton;
boolean etatLed;
int flag;

void setup()
{
  // Préparation et initialisation des équipements
  lcd.begin(2,16);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(BTNSTART, INPUT_PULLUP);
  pinMode(BTNACT, INPUT_PULLUP);
  Serial.begin(9600); // Pour test console
  randomSeed(analogRead(A0)); // Initialise un pseudo nombre aléatoire
  state = -1;
}

void loop()
{
  byte newstate; // Afin de garder "l'état" stable
  
  newstate = state;
  
  // Machine d'état du programme
  switch(state)
  {
    case -1: // Init du programme, allumage et remise à 0
    lcd.clear();
    lcd.home();
    lcd.print(F("ACTIVER START"));
    newstate = 0;
    break;
    
    case 0: // Detection de l'activation du bouton start et lancement du jeu
    etatBouton = digitalRead(BTNSTART);    
    if(etatBouton == 0)
    {
      lcd.clear();
      lcd.home();  
      lcd.print(F("START ATTENTION")); // On previent l'utilisateur
      digitalWrite(LEDGREEN,HIGH); // On allume la led verte
      newstate = 1;
    }     
    break;
    
    case 1: // Activation du jeu + calcul du temps de refléxe 
    delay(random(5000,20000)); // Délai aléatoire entre 5 et 20 secondes    
    digitalWrite(LEDRED,HIGH); // Après ce délai ==> Led rouge s'allume  
    etatLed = digitalRead(LEDRED);
    if(etatLed == HIGH)
    {
      time1 = millis();
      flag = 1;
    }
    
    while(flag == 1)
    {
      if(!digitalRead(BTNACT))
      {
        time2 = millis();               
        lcd.clear();
        lcd.home();    
        timeTotal = (time2 - time1); 
        lcd.print("Temps : ");
        lcd.print(timeTotal);
        timeTotal = 0;
        time1 = 0;
        time2 = 0;   
        flag = 0;
        break;
      }   
    }   
    
    digitalWrite(LEDGREEN,LOW); //On éteint les leds
    digitalWrite(LEDRED,LOW);
    newstate = 2;    
    break;
    
    case 2 : // Retour écran de départ
    etatBouton = digitalRead(BTNSTART);
    if(etatBouton == 0)
    {
      lcd.clear();
      lcd.home(); 
      newstate = -1;
      delay(1000); // Solution brutal pour éviter de passer trop vite de -1 à 0
    }      
    break;
  }
  state = newstate; // Récup de l'état
}
