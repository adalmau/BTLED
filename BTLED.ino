#include <SoftwareSerial.h>

SoftwareSerial BTSerial(11, 10); // RX | TX

#define END_CMD_CHAR '\n'
#define RET_CMD_CHAR '\r'

void setup()
{ 
  /*
   * El pin 9 serveix par alimentar el mòdul BT. Aquests tipus de mòduls funcionen de 3.6v a 6v.
   * Si el connectem al pin 9 el seu voltatge serà de 3.3v o 5v depenent de l'arduino. Si vulguéssim 
   * sempre treballar a 5v l'hauríem de connectar a aquell pin de l'arduino que tingués aquest voltatge.
   * Encara que puguem treballar a 3.6v, millor treballar a 5v.
   */
  pinMode(9, OUTPUT);  
  digitalWrite(9, HIGH);
  
  /*
   * En el meu mòdul BT no cal fer aquesta connexió ja que té botó físic.
   * Per entrar en AT mode cal mantenir el botó apretat mentre se li dóna alimnetació.
   * En mòduls BT que no tinguin botó físic cal enviar la senyal HIGH al pin KEY
   * del propi mòdul. Per fer-ho utilitzem un pin de l'arduino, en aquest cas fóra el 8
   * pinMode(8, OUTPUT);  
   * digitalWrite(8, HIGH);
   */

  // Inicialitzar el port sèrie per intercomunicar-nos amb el PC
  Serial.begin(9600);
  Serial.println("Enter AT commands:");

  // Inicialitzar el port sèrie del bluetooth per intercomunicar-nos amb el mòbils, PCs, etc.
  BTSerial.begin(38400);  // HC-05 default speed in AT command mode.
  // Fer que els pins 5, 6 i 7 siguin de sortida perquè és on es connectarà el LED RGB
  for (int i = 5 ; i <= 7 ; i++)
    pinMode(i, OUTPUT);
}

void loop()
{
  if (BTSerial.available())
    {
      /*
       * Tot allò que el port del Bluetooth rebi ho mostrarà pel port sèrie i podrà ser llegit per la consola,
       * ja siguin comandes AT o ja siguin colors enviats per bluetooth
       */
      String inMSG = GetLine();
      Serial.println(inMSG);
      splitColorAndSend(inMSG);
    }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());

  
}

String GetLine()
{   
  String msg = "" ;
  if (BTSerial.available())
  {  
    char c = BTSerial.read();
    while (c != END_CMD_CHAR)
    {  
      msg = msg + c ;
      delay(25) ;
      c = BTSerial.read();
    }
  }
  return(msg);
}


void Color(int R, int G, int B)
{     
  analogWrite(5, R) ;   // Red    
  analogWrite(6, G) ;   // Green
  analogWrite(7, B) ;   // Blue
}

void splitColorAndSend(String color)
{
  int i = 0;
  int pos = 0;
  int arrColor[] = {0,0,0};
  int arrColorIndex = 0;

  for (i=0; i<=color.length(); i++)
  {
    if (color[i] == ',' || i == color.length())
    {
      arrColor[arrColorIndex] = (color.substring(pos,i)).toInt();
      pos = i+1;
      arrColorIndex++;
    }
  }

  Color(arrColor[0],arrColor[1], arrColor[2]);

  Serial.println(arrColor[0]);
  Serial.println(arrColor[1]);
  Serial.println(arrColor[2]);
}
