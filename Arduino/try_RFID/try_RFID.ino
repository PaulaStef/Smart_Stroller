#include <RFID.h>
/*
PINOUT:

RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V

*

* Include the standard Arduino SPI library */
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
// pini pentru RFID reader
#define SDA_DIO 53
#define RESET_DIO 8

// pini pentru modulul bluetooth
#define BluetoothTx 11
#define BluetoothRx 10

SoftwareSerial BluetoothSerial(BluetoothRx, BluetoothTx);

// sirul in care am pastrat ID-ul fiecarui card
int productID[5]={209,68,67,195,12};

//un sir in care am specificat pretul fiecarui produs
int productPrice[5]={14,12,15,12,16};

//un sir in care am assignat un nume de produs fiecarui ID
String names[] = {"Apple", "Bread", "Water", "Juice", "Orange"};

//un sir in care pastram fiecare cantitate
int quantity[5]={0,0,0,0,0};

//Pretul final pentru cumparaturi
int Total;
int k = 0;

//un sir in care vom pune produsele adaugate in cosul de cumparaturi
String addedProducts[5];

//Am creat o instanta a librariei
RFID RC522(SDA_DIO, RESET_DIO); 

//Pini folositi pentru butoane
const int buttonPin1 = 2;   
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;
const int buttonPin5 = 12;

//pini folositi pentru led-uri
int LED = 6;
int LED2 =9;

//pinul pentru buzzer
const int buzzerPin = 7;

// variabile care imi vor specifica  starea fiecarui buton - HIGH cand e apasat si LOW cand nu
int buttonState1 = 0;         
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;

//o variabila prin care am numarat de cate ori este apasat butonul 4
int checkState;

void setup()
{ 
  Serial.begin(9600);
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  // am initializat pinii de intrare
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT); 
  pinMode(buttonPin5, INPUT);
  //am initializat pinii de iesire pentru led-uri
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  // am setat valorile initiale pentru led-uri
  digitalWrite(LED, LOW); //RED
  digitalWrite(LED2, HIGH); //GREEN
  //am setat pinul pentru buzzer ca pin de iesire
  pinMode(buzzerPin, OUTPUT);
  BluetoothSerial.begin(9600);
  /* Am pornit interfata SPI*/
  SPI.begin(); 
  /* Am initializat RFID-ul */
  RC522.init();
  //am initializat numarul de apasari ale butonului4
  checkState = 0;

}

void loop()
{
  //citim starile butoanelor 4 si 5
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  checkGiveUp();
  checkList();
  analogWrite(buzzerPin ,255);
  //digitalWrite(LED, LOW);
  //digitalWrite(LED2, HIGH);

  byte i=0;
  byte j=0;
  int ID;

  //Aici verificam daca a fost citit un card
  if (RC522.isCard())
  { 
    buttonState4 = digitalRead(buttonPin4); 
    boolean stay = true;
    analogWrite(buzzerPin,0);
    digitalWrite(LED, HIGH);
    digitalWrite(LED2, LOW);

    /* Daca da, ii preluam ID-ul */
    RC522.readCardSerial();
    
    /* Verificam daca avvem ID-ul  */
    ID=RC522.serNum[0];
    for(i=0;i<5;i++)
    { 
      buttonState4 = digitalRead(buttonPin4);
      if(productID[i]==ID)
      {
        printItem(i);
        if(quantity[i]==0)
        {
          quantity[i]++;
          printItem(i);
        }
        while(stay == true)
        {
          buttonState1 = digitalRead(buttonPin1);
          buttonState2 = digitalRead(buttonPin2);
          buttonState3 = digitalRead(buttonPin3);
          
          stay = checkButton(buttonState1, 1, i, stay);
          delay(100);
          stay = checkButton(buttonState2, 2, i, stay);
          delay(200);
          stay = checkButton(buttonState3, 3, i, stay);
          delay(100);
          
        }
        break;
      }
      else if(i==5)
      { 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Product not found"); 
        break;
      }
    }
    //checkList();
  }
  else{
    //Serial.print("Reading...");
  }
  checkList();
  delay(1000);
}


void printItem(int pos){
  //firstline
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print(names[pos]);
  lcd.setCursor(13,0);
  lcd.print(productPrice[pos]);
  lcd.setCursor(15,0);
  lcd.print("$");

  //secondline
  lcd.setCursor(0,1);
  lcd.print("x");
  lcd.setCursor(1,1);
  lcd.print(quantity[pos]);
  lcd.setCursor(12,1);
  int total = quantity[pos] * productPrice[pos];
  lcd.print(total);
  lcd.setCursor(15,1);
  lcd.print("$");
}

void checkGiveUp(){
  if( digitalRead(buttonPin5) == HIGH){
      analogWrite(buzzerPin ,0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Drooping items");
      lcd.setCursor(0,1);
      lcd.print("List Emptied");
      delay(1000);
      for(int j=0; j<5; j++){
        addedProducts[j] = "";
        quantity[j] = 0;
      }
       Total = 0;
       k=0;
       digitalWrite(LED, LOW);
       digitalWrite(LED2, HIGH);
       //lcd.clear();
       checkState = 0;
  }else{
    analogWrite(buzzerPin,255);
  }
}
void checkList(){
  if ( digitalRead(buttonPin4) == HIGH){
    if(checkState == 0){
     for(int j =0; j<k; j+=2){
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(addedProducts[j]);
       lcd.setCursor(0,1);
       lcd.print(addedProducts[j+1]);
       delay(2000);
     }
     lcd.clear();
     lcd.print("Total: " + String(Total) + "$");
     delay(2000);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Press again for");
     lcd.setCursor(0,1);
     lcd.print("checkout");
     checkState = 1;
    }
    else if(checkState == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Your total is: ");
      lcd.setCursor(0,1);
      lcd.print(String(Total) + "$");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press again to");
      lcd.setCursor(0,1);
      lcd.print("pay items");
      checkState = 2;
    }
    else if(checkState == 2){
      analogWrite(buzzerPin ,0);
      lcd.clear();
      int paied = Total;
      lcd.print("You paid " + String(Total) + "$");
      delay(2000);
      for(int j=0; j<k; j++){
       Serial.println(addedProducts[j]);
      }
      for(int j=0; j<5; j++){
        addedProducts[j] = "";
        quantity[j] = 0;
      }
       Total = 0;
       k=0;
       digitalWrite(LED, LOW);
       digitalWrite(LED2, HIGH);
       //lcd.clear();
       checkState = 0;
    }
  }else {
     analogWrite(buzzerPin,255);
  }
}

boolean checkButton( int buttonState, int buttonNumber, int pos, boolean state){
  if (buttonState == HIGH) {
    analogWrite(buzzerPin ,0); 
    //Button1
    if(buttonNumber == 1){
      //Crestem cantitatea
      quantity[pos]++;
      printItem(pos);
    }
    //Button2
    if(buttonNumber == 2){
      //Scadem cantitatea
      quantity[pos]--;
      printItem(pos);
    }
    //Button3
    if(buttonNumber == 3){
      //Se salveaza modificarile si se trece la urmatorul buton
      Total = 0;
      for(int j=0; j<5; j++){
        if(quantity[j] != 0)
          Total += quantity[j] * productPrice[j];
      }
       lcd.clear(); 
       
       //first line
       lcd.setCursor(0,0);
       String message1 = String(quantity[pos]);
       message1 += " x " + names[pos] + " added"; 
       lcd.print(message1);
       
       //second line
       lcd.setCursor(0,1);
       String message2 = "Total is: " + String(Total) + "$";
       lcd.print(message2);
       //Add item to cart
       if(quantity[pos] != 0){
         int flag = 0;
         for(int j=0; j<k; j++){
           if(addedProducts[j].startsWith(names[pos])){
            addedProducts[j] = names[pos] + " x" + String(quantity[pos]);
            flag = 1;
            break;
           }
         }
         if(flag == 0){
           addedProducts[k] = names[pos] + " x" + String(quantity[pos]);
           k++;
         }
       }
       else{ //if quantity is 0 we want to delete the item
        for(int j=0; j<k; j++)
         if(addedProducts[j].startsWith(names[pos]))
         {
            for(int m=j; m<k; m++)
              addedProducts[m] = addedProducts[m+1];
            k--;
            break;
         }
       }
       state = false;
    }
  }else {
    analogWrite(buzzerPin,255);
  }
  return state;
}
