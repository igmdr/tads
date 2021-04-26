
#include <HX711.h>

HX711 celCarga1;
HX711 celCarga2;
HX711 celCarga3;

float soma1 = 0, soma2 = 0, soma3 = 0, fatorCal1 = 1, fatorCal2 = 1, fatorCal3 = 1, spd = 90;
float *p = 0, tempo = 0, tempoAtual = 0;
int pinoSensor = 9, estadoSensor = 0, rpm = 0, cont = 0, rot=0, encoder=0;
bool flag = true;

class DCMotor{  
  int pin1, pin2;
  
  public:
  void Pinout(int in1, int in2){// Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
    pin1 = in1;
    pin2 = in2;
    pinMode(pin1,OUTPUT);
    pinMode(pin2,OUTPUT);
  }
  void Speed(int input){// Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
    spd = input;
    Forward();
  }     
  void Forward(){// Forward é o método para fazer o motor girar para frente
    analogWrite(pin1, spd);    
    digitalWrite(pin2, LOW);
  }   
  void Backward(){// Backward é o método para fazer o motor girar para trás
    digitalWrite(pin1, LOW);    
    analogWrite(pin2, spd);
  }
  void Stop(){// Stop é o metodo para fazer o motor ficar parado
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
};

DCMotor Motor1;// Criação de do objeto motor já pronto para receber os comandos configurados acima 

void zerarBalanca(){
  celCarga1.tare(); //zera balança
  celCarga2.tare();
  celCarga3.tare();
}

void selecionarOpcao(char temp){
  if(isDigit(temp)){
    if(temp == '1'){
      p = &fatorCal1;
    }
    else if(temp == '2'){
      p = &fatorCal2;
    }
    else if(temp == '3'){
      p = &fatorCal3;
    }
    else if(temp == '4'){
      p = &spd;
    }
  }
  else if (temp == 'a'){
    *p += 1;
  }
  else if(temp == 's'){
    *p += 10;
  }
  else if (temp == 'd'){
    *p += 100;
  }
  else if (temp == 'f'){
    *p += 1000;
  }
  else if (temp == 'z'){
    *p += -1;
  }
  else if(temp == 'x'){
    *p += -10;
  }
  else if (temp == 'c'){
    *p += -100;
  }
  else if (temp == 'v'){
    *p += -1000;
  }
  else if (temp == 't'){
    zerarBalanca();
  }
  else if (temp == 'i'){
    Serial.println("fim");
  }
}

void controlarMotor(bool flag){
  if(flag == true){
    Motor1.Forward();
  }
  else{
    Motor1.Backward();
  }
}

void setup(){
  Serial.begin(115200);
  celCarga1.begin(3,2);
  celCarga2.begin(4,7);
  celCarga3.begin(10,11);

  Motor1.Pinout(5,6);// Seleção dos pinos que cada motor usará, como descrito na classe.
  controlarMotor(flag);
  
  pinMode(pinoSensor, INPUT);//Define o pino como entrada

  celCarga1.set_scale();//This value is obtained by using the SparkFun_HX711_Calibration sketch
  celCarga2.set_scale();
  celCarga3.set_scale();
  zerarBalanca();
}

void loop(){

  if(Serial.available()){// reconhece letra para ajuste do fator de calibração
  selecionarOpcao(Serial.read());
  }
  
  celCarga1.set_scale(fatorCal1);
  celCarga2.set_scale(fatorCal2);
  celCarga3.set_scale(fatorCal3);
  soma1 = celCarga1.get_units(1);//scale.get_units() returns a float
  soma2 = celCarga2.get_units(1);
  soma3 = celCarga3.get_units(1);

  estadoSensor = digitalRead(pinoSensor);

  Motor1.Speed(spd);// A velocidade do motor pode variar de 0 a 255, onde 255 é a velocidade máxima
  
    /*Serial.print(soma1/100, 2); soma1 = 0;//divide por 100 para transformar mg em gramas e por 10 para determinar a media da soma1
    
    Serial.print("; ");//pode ser mudado para g mas é necessário refatorar o fator de calibração
    Serial.print(soma2/100, 2); soma2 = 0;
     
    Serial.print("; ");
    Serial.print(soma3/100, 2); soma3 = 0;
    
    Serial.print("; ");*/
    Serial.print(estadoSensor); //rot = 0; cont = 0;
    
    Serial.print("; ");
    Serial.print(millis());
    
    Serial.print("; ");
    Serial.println(spd*0.3921568627);
  
}
