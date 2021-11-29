// https://cdn.awsli.com.br/969/969921/arquivos/ph-sensor-ph-4502c.pdf
#define PH_PIN A0
#define TEMP_PIN A1

// pH calibration value
float calibration = 23.60;

void setup() {
	Serial.begin(9600);
}

void loop() {
	float pHValue = getPH();
	float tempValue = getTemperature();

	Serial.println("pH: ");
	Serial.println(pHValue);

	Serial.print("Temperature: ");
	Serial.print(tempValue);
	Serial.println(" C");

	delay(1000);
}


float getPH(){
	int buf[10], temp;
	unsigned long int avgValue;
	float pHVol, pHValue;

	// Reading the values
	for(int i=0;i<10;i++)
	{
		buf[i]=analogRead(PH_PIN);
		delay(30);
	}

	// Sorting the values in ascending order
	for(int i=0;i<9;i++)
		{
		for(int j=i+1;j<10;j++)
		{
		if(buf[i]>buf[j])
		{
			temp=buf[i];
			buf[i]=buf[j];
			buf[j]=temp;
			}
		}
	}

	avgValue=0;
	for(int i=2;i<8;i++)
		avgValue+=buf[i];

	pHVol=(float)avgValue*5.0/1024/6;
	pHValue = -5.70 * pHVol + calibration;

	return pHValue;
}

float getTemperature(){
	// Termistor parameters
	const double beta = 10000.0;
	const double r0 = 10000.0;
	const double t0 = 273.0 + 25.0;
	const double rx = r0 * exp(-beta/t0);
	 
	// Circuit parameters
	const double vcc = 5.0;
	const double R = 20000.0;
	 
	const int nSamples = 5;

	int sum = 0;
		for (int i = 0; i < nSamples; i++) {
		sum += analogRead(TEMP_PIN);
		delay (10);
	}

	double v = (vcc*sum)/(nSamples*1024.0);
	double rt = (vcc*R)/v - R;
	 
  	double t = beta / log(rt/rx);
	return t-273.0;
}