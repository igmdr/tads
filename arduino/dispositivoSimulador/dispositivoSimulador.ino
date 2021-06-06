
float *p = 0, spd = 125, rot = 0;
const byte pinoSensor = 2;
long tempo = 0, tempoAtual = 0, intervaloAcumulado = 0;
bool flag = true;
int ultimaLeitura = 0;

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

DCMotor Motor1;// Criação do objeto motor já pronto para receber os comandos configurados acima 

void selecionarOpcao(char temp){
  if(isDigit(temp)){
    if(temp == '4'){
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
  
  Serial.begin(9600);

  Motor1.Pinout(5,6);// Seleção dos pinos que cada motor usará, como descrito na classe.
  controlarMotor(flag);

  pinMode(pinoSensor,INPUT);

}

void loop(){

  if(Serial.available()){// reconhece letra para ajuste da velocidade
    selecionarOpcao(Serial.read());
  }

  Motor1.Speed(spd);// A velocidade do motor pode variar de 0 a 255, onde 255 é a velocidade máxima
  
  int temp = digitalRead(pinoSensor);
  if(temp != ultimaLeitura){
    ultimaLeitura = temp;
    rot += 0.5;
  }

  tempoAtual = millis();// retorna o tempo de execução da aplicação em milisegundos
  long intervalo = (tempoAtual - tempo);tempo = tempoAtual;
  intervaloAcumulado += intervalo;//determina o tempo entre duas amostras

  Serial.print(temp);
  Serial.print("; ");
  
  Serial.print(tempoAtual);
  Serial.print("; ");
  
  Serial.print(rot);

  if(intervaloAcumulado>=1000){
    if(rot < 4.5){
      spd += 3;
    }
    if(rot > 5.5){
      spd -= 2;
    }
    Serial.print("; ");
    Serial.print(rot); rot = 0;
    Serial.print("; ");
    
    Serial.print(intervaloAcumulado); intervaloAcumulado = 0;
    Serial.print("; ");
    
    Serial.println(spd*0.3921568627);// calcula o valor do pwm de 0 a 100%    
  }
  else{
    Serial.println();
  }

}
