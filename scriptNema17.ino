#include <EEPROM.h>

#define dir 11
#define pasos 13
#define enable 12

#define FinCarreraSup 7
#define FinCarreraInf 6
#define BotReset 4
#define BotComenzar 3
#define BotSeleccion 2
#define lapsoTiempo 600
#define V_salpicaduras 900

int velocidad,t0;

void movimiento_motor()
{ 
digitalWrite(enable,LOW); //habilito el motor
if (digitalRead(FinCarreraInf)==LOW)
{
digitalWrite(pasos,HIGH);
delayMicroseconds(velocidad);
digitalWrite(pasos,LOW);
delayMicroseconds(velocidad);    
}
digitalWrite(enable,HIGH); //deshabilito el motor
}

void movimientoHaciaOrigen()
{
while(digitalRead(FinCarreraSup)==LOW) //mientras el FC esté abierto
{
digitalWrite(dir,HIGH); //direcciono el movimiento hacia adelante/abajo
//Serial.println("Subiendo motor hasta que cierre el final de carrera");
velocidad=800;
movimiento_motor();
}
digitalWrite(dir,LOW); //direcciono el movimiento hacia arriba/atrás
}

void setup() {
  Serial.begin(9600);
  pinMode(dir,OUTPUT);
  pinMode(pasos,OUTPUT);
  pinMode(enable,OUTPUT);
  digitalWrite(enable,HIGH);  
  
  pinMode(FinCarreraSup,INPUT); //pin7
  pinMode(FinCarreraInf,INPUT); //pin6
  pinMode(BotComenzar,INPUT);  //pin3
  pinMode(BotSeleccion,INPUT); //pin2
  pinMode(BotReset,INPUT); //pin4
}

void loop() {
//------------POSICIONAMIENTO INICIAL---------//
movimientoHaciaOrigen(); // primera etapa del ensayo, se vuelve a origen
//--------------------------------------------//

while(digitalRead(FinCarreraInf)==LOW) //mientras el FC esté abierto
{
digitalWrite(enable,HIGH); //se deshabilitan los motores
//-------------SI SE RESETEA------------------//
if (BotReset==HIGH) {movimientoHaciaOrigen();} //si se apreta RESET vuelve a origen
Serial.println("FCInferiorAbierto"); //impresión solo con motivos de debug, se puede anular
//--------------------------------------------//

if (digitalRead(BotComenzar)==HIGH)
{
t0=millis();
Serial.println("El ensayo a comenzado");
while((millis()-t0)<lapsoTiempo)
{
//Serial.println("Bajando hasta que cierre el final de carrera de abajo");
if (digitalRead(FinCarreraInf)==LOW) //chequeo nuevamente que no haya llegado al final
{ 
movimiento_motor();
}
tiempo=millis()-t0;
}//endWhile salpicadura

if (t0>0)
{
char buffer[50];  
sprintf(buffer,"La salpicadura ha finalizado, en: %d",tiempo);
Serial.println(buffer);
}

}//endif botonPresionado
}//endWhile ciclo de ensayo
}//endLoop
 /////////////////////////////////FIN DE LOOP/////////////////////////////////////////
