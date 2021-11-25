const int sensorPin = A2; //PINO ANALÓGICO UTILIZADO PELO SENSOR
float temperatura = 0; //VARIÁVEL DO TIPO FLOAT
               
void setup(){
  Serial.begin(9600); //INICIALIZAÇÃO DA SERIAL
}
 
void loop(){
   temperatura = (analogRead(sensorPin) * 0.0048828125 * 100); //VARIÁVEL RECEBE A TEMPERATURA MEDIDA
   Serial.print("Temperatura = "); //IMPRIME O TEXTO NA SERIAL
   Serial.print(temperatura); //IMPRIME NA SERIAL A TEMPERATURA MEDIDA
   Serial.println(" C"); //IMPRIME O TEXTO NA SERIAL
   delay(2000); //INTERVALO DE 2 SEGUNDOS
 }