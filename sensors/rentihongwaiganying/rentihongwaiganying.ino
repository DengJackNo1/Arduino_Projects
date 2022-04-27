int value;
long ttime;
void setup()
{
 Serial.begin(9600);
 pinMode(6,INPUT);
 
}
void loop()
{
 value=digitalRead(6);
 Serial.print(value );
 if(value == 1){
 Serial.println("Nobody");
 }
 else{
 Serial.println("Someone is here");
 }
 delay(100);
}
