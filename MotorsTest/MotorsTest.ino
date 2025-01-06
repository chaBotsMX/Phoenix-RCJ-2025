int PWMA = 9, PWMB = 6, PWMC = 29, PWMD = 3;
int MA1 = 28, MA2 = 8, MB1 = 7, MB2 = 5, MC1 = 31, MC2 = 30, MD1 = 4, MD2 = 2;

void setup() {
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  pinMode(MD1, OUTPUT);
  pinMode(MD2, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(PWMC, OUTPUT);
  pinMode(PWMD, OUTPUT);
}

void loop() {
  digitalWrite(MA1, LOW); digitalWrite(MA2, HIGH);
  analogWrite(PWMA, 150); 
}

void adelante(int PWM){
  digitalWrite(MA1, LOW);digitalWrite(MA2, HIGH);
  analogWrite(PWMA,PWM); 

  digitalWrite(MB1, HIGH);digitalWrite(MB2, LOW);
  analogWrite(PWMB,PWM);

  digitalWrite(MC1, LOW);digitalWrite(MC2, HIGH); 
  analogWrite(PWMC,PWM);

  digitalWrite(MD1, HIGH);digitalWrite(MD2, LOW); 
  analogWrite(PWMD,PWM);
}

void stop(){
  digitalWrite(MA1, LOW);digitalWrite(MA2, LOW);
  analogWrite(PWMA,0); 

  digitalWrite(MB1, LOW);digitalWrite(MB2, LOW);
  analogWrite(PWMB,0);

  digitalWrite(MC1, LOW);digitalWrite(MC2, LOW); 
  analogWrite(PWMC,0);
  
  digitalWrite(MD1, LOW);digitalWrite(MD2, LOW); 
  analogWrite(PWMD,0);
}