
#include <HX711.h>

HX711 celCarga1;
HX711 celCarga2;
HX711 celCarga3;

float *p = 0, cel1 = 0, cel2 = 0, cel3 = 0, soma1, soma2, soma3, fatorCal1 = 1, fatorCal2 = 1, fatorCal3 = 1;
long tempo = 0, tempoAtual = 0, intervaloAcumulado = 0;
int cont = 0;

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

void setup(){
  Serial.begin(9600);
  celCarga1.begin(3,4);//parâmetros(dt,sck)
  celCarga2.begin(9,8);
  celCarga3.begin(10,11);

  celCarga1.set_scale(1000);
  celCarga2.set_scale(1000);
  celCarga3.set_scale(1000);
  zerarBalanca();
}

void loop(){

  if(Serial.available()){// reconhece letra para ajuste do fator de calibração
  selecionarOpcao(Serial.read());
  }
  /*celCarga1.set_scale(fatorCal1);
  celCarga2.set_scale(fatorCal2);
  celCarga3.set_scale(fatorCal3);*/
  cel1 = celCarga1.get_units();//scale.get_units() retorna valor float
  cel2 = celCarga2.get_units();
  cel3 = celCarga3.get_units();
  soma1 += cel1;
  soma2 += cel2;
  soma3 += cel3;
  cont++;
  
  Serial.print(cel1, 1);//valor de massa em mg
  Serial.print("; ");
  Serial.print(cel2, 1);
  Serial.print("; ");
  Serial.print(cel3, 1);
  Serial.print("; ");
  
  tempoAtual = millis();// retorna o tempo de execução da aplicação em milisegundos
  long intervalo = (tempoAtual - tempo);tempo = tempoAtual;
  intervaloAcumulado += intervalo;//determina o tempo entre duas amostras

  Serial.print(tempoAtual);

  if(intervaloAcumulado>=1000){
    Serial.print("; ");
    Serial.print(soma1/cont); soma1 = 0;
    Serial.print("; ");
    Serial.print(soma2/cont); soma2 = 0;
    Serial.print("; ");
    Serial.print(soma3/cont); soma3 = 0; cont = 0;
    Serial.print("; ");
    
    Serial.print(intervaloAcumulado); intervaloAcumulado = 0;
    Serial.println();   
  }
  else{
    Serial.println();
  }

}
