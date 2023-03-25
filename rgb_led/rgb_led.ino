#define rpin 18
#define gpin 19
#define bpin 26
void setup() {
  // put your setup code here, to run once:
   
   ledcSetup(0, 256, 8);
   ledcSetup(1, 256, 8);
   ledcSetup(2, 256, 8);
   ledcAttachPin(rpin, 0);
   ledcAttachPin(gpin, 1);
   ledcAttachPin(bpin, 2);

}
void color(int r, int g, int b){
   ledcWrite(0, r);
   ledcWrite(1, g);
   ledcWrite(2, b);
   
}
void loop() {
  // put your main code here, to run repeatedly:
   for (int i=0; i<256; ++i){
      color(255-i, i, 0);
      delay(10);
   }
   for (int i=0; i<256; ++i){
      color(0, 255-i, i);
      delay(10);    
}  
   for(int i = 0; i<256;++i){
      color(i, 0, 255-i);
      delay(10);      
   }
}
