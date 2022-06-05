#include <SPI.h> // Not actually used but needed to compile
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <RH_ASK.h> // RF Library

RH_ASK rf_driver;

extern uint8_t qr[]; // QR code Bitmap array.

#define YP A2 
#define XM A1 
#define YM 6
#define XP 7 
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define RED 0xF800
//#define CYAN 0x07FF
#define BLUE 0x001F
//#define PURPLE 0x780F
//#define YELLOW 0xFFE0
//#define LIME 0x07E0
#define WHITE 0xFFFF
//#define GREY 0x7BEF
#define BLACK 0x0000
#define ORANGE 0xFD00
//#define MAROON 0x7800
//#define GREEN 0x07E0
//#define OLIVE 0x7BE0
#define BLUE_GRAY 0x9578
#define ROYAL_BLUE 0x62FC
#define LIME_GREEN 0x3D02
#define CRIMSON 0xD9BC


//Minimum and maximum pressure to sense the touch
#define MINPRESSURE 10
#define MAXPRESSURE 1000

char currentpage; 
/* 
 Home Page = H
 Burger Menu = B
 Pizza Menu = P
 Hot Dogs = D
 Fries Menu = F
 Drinks Menu = R
 Help Menu = W
 Checkout = C
 QR page = Q
 
 */

short totalBill = 0;
short bill = 0;
String orderList1="";
//String orderList2;
String orderListFinal="";
String orderCheckout="";
//char* conv_orderListFinal="";


void drawHome()
{
  tft.fillScreen(WHITE);
  //tft.drawRoundRect(0, 0, 319, 240, 8, WHITE);     //Page border
  tft.fillRoundRect(30, 40, 120, 40, 8, ORANGE); //Burgers
  tft.drawRoundRect(30, 40, 120, 40, 8, BLACK);  //Burgers Border
  tft.fillRoundRect(30, 90, 120, 40, 8, ORANGE); //Pizza
  tft.drawRoundRect(30, 90, 120, 40, 8, BLACK);  //Pizza Border
  tft.fillRoundRect(30, 140, 120, 40, 8, ORANGE);   //Hot Dogs
  tft.drawRoundRect(30, 140, 120, 40, 8, BLACK);    //Hot Dogs Border
  tft.fillRoundRect(30, 190, 120, 40, 8, ORANGE);   // Drinks
  tft.drawRoundRect(30, 190, 120, 40, 8, BLACK); // Drinks Border
  tft.fillRoundRect(180, 40, 120, 40, 8, 0x62FC);   //Help
  tft.drawRoundRect(180, 40, 120, 40, 8, BLACK);  //Help Border
  tft.fillRoundRect(180, 90, 120, 40, 8, RED); // //Checkout
  tft.drawRoundRect(180, 90, 120, 40, 8, BLACK); // Border
  
  
  tft.setCursor(60, 10);
  tft.setTextSize(3);
  tft.setTextColor(RED);
  tft.print("   Menu");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(37, 47);
  tft.print(" Burgers");
  tft.setCursor(37, 97);
  tft.print(" Pizza");
  tft.setCursor(37, 147);
  tft.print(" Hot Dogs");
  tft.setCursor(37, 197);
  tft.print(" Drinks");
  tft.setCursor(187, 47);
  tft.print(" Help");
  tft.setCursor(187, 97);
  tft.print(" Checkout");
  delay(500);
}
void setup()
{
  tft.reset();
  tft.begin(tft.readID());
  Serial.begin(9600);
  Serial.println();
  //Serial.print("reading ID...");
  delay(500);
  Serial.println(tft.readID(),HEX);
  //tft.fillScreen(WHITE);
  tft.setRotation(1);

 
  tft.fillScreen(WHITE);
  drawHome();
  currentpage = 'H'; 
  rf_driver.init(); // RF Initialization.
}

  void loop()
  {
    digitalWrite(13,HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13,LOW);

    pinMode(XM,OUTPUT);
    pinMode(YP,OUTPUT);
     //Display Touch coordinates and pressure to the Serial console  
//      Serial.print("p.x = ");
//      Serial.print(p.x);
//      Serial.print("  |  p.y = ");
//      Serial.print(p.y);
//      Serial.print("  |  Pressure = ");
//      Serial.println(p.z); 

   
    
    if (currentpage == 'H')
      {
        if (p.x > 647 && p.x < 725 && p.y > 300 && p.y < 460 && p.z > MINPRESSURE && p.z < MAXPRESSURE)
         {
          // Serial.println("Burgers Menu");
          
          
           tft.fillRoundRect(30, 40, 120, 40, 8, WHITE);
           delay(70);

           tft.fillRoundRect(30, 40, 120, 40, 8, ORANGE);
           tft.drawRoundRect(30, 40, 120, 40, 8, BLACK);
           tft.setCursor(37, 47);
           tft.print(" Burgers");
           delay(70);

           drawBurgers();  
           delay(70); 
           
           currentpage = 'B'; 
           }
         
       if (p.x > 490 && p.x < 561 && p.y > 280 && p.y < 470 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Pizza Menu
          {
           //Serial.println("Pizza Menu");
           
           tft.fillRoundRect(30, 90, 120, 40, 8, WHITE);
           delay(70);

           tft.fillRoundRect(30, 90, 120, 40, 8, ORANGE);
           tft.drawRoundRect(30, 90, 120, 40, 8, BLACK);
           tft.setCursor(37, 97);
           tft.print(" Pizza");
           delay(70);
          
           drawPizza(); 
           delay(1000); 
           currentpage = 'P';    
         }
      
       if (p.x > 320 && p.x < 415 && p.y > 275 && p.y < 522 && p.z > MINPRESSURE && p.z < MAXPRESSURE)
        {
          //Serial.println("Hot Dogs Menu");
          tft.fillRoundRect(30, 140, 120, 40, 8, WHITE);
          delay(70);

          tft.fillRoundRect(30, 140, 120, 40, 8, ORANGE);
          tft.drawRoundRect(30, 140, 120, 40, 8, BLACK);
          tft.setCursor(37, 147);
          tft.print(" Hot Dogs");
          delay(70);

          drawHotDogs();  
          currentpage = 'D';  
       }
       
      if (p.x > 160 && p.x < 275 && p.y > 275 && p.y < 520 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Drinks Menu
        {
          //Serial.println("Fries Menu");
          tft.fillRoundRect(30, 190, 120, 40, 8, WHITE);
          delay(70);

          tft.fillRoundRect(30, 190, 120, 40, 8, ORANGE);
          tft.drawRoundRect(30, 190, 120, 40, 8, BLACK);
          tft.setCursor(37, 197);
          tft.print(" Drinks");
          delay(70);

          drawDrinks();   
          currentpage = 'R'; 
       }
       
     if (p.x > 630 && p.x < 720 && p.y > 630 && p.y < 870 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Help
       {
          //Serial.println("Drinks Menu");
          tft.fillRoundRect(180, 40, 120, 40, 8, WHITE);
          delay(70);

          tft.fillRoundRect(180, 40, 120, 40, 8, 0x62FC);
          tft.drawRoundRect(180, 40, 120, 40, 8, BLACK);
          tft.setCursor(187, 47);
          tft.print(" Help");
          delay(70);

          drawHelp(); 
          currentpage = 'W';   
          
          const char *msg = "Help Wanted  "; //Help Code

          rf_driver.send((uint8_t *)msg, strlen(msg));
          rf_driver.waitPacketSent();
          delay(200);
       }
       
     if (p.x > 480 && p.x < 570 && p.y > 640 && p.y < 870 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Checkout
        {
        
          tft.fillRoundRect(180, 90, 120, 40, 8, WHITE);
          delay(70);

          tft.fillRoundRect(180, 90, 120, 40, 8, RED);
          tft.drawRoundRect(180, 90, 120, 40, 8, BLACK);
          tft.setCursor(187, 97);
          tft.print(" Checkout");
          delay(70);

          drawCheckout();   
          currentpage = 'C'; 
       }
    
    
   } //Home Page Block Ends Here


  if (currentpage == 'B') //Burger Menu
        {
          if(p.x > 160 && p.x < 275 && p.y > 275 && p.y < 520 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Home Button
            {
                
               //Serial.println("Home");
               tft.fillRoundRect(30, 190, 120, 40, 8, WHITE);
               delay(70);

               tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);
               tft.drawRoundRect(30, 190, 120, 40, 8, BLACK);
               tft.setCursor(37, 197);
               tft.print("  Home");
               delay(70);
                
               drawHome();
               currentpage = 'H';
             } 

           if (p.x > 640 && p.x < 725 && p.y > 490 && p.y < 720 && p.z > MINPRESSURE && p.z < MAXPRESSURE) // Chicken Burger
              {
                tft.fillRoundRect(30, 40, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);
                tft.setCursor(37, 47);
                tft.print(" Chicken Burger");
                delay(70);

                bill = 120;
                totalBill = totalBill + bill;

                

               orderCheckout = "  Chicken Burger\t\t\t\t120\n" + orderCheckout;
               orderList1 = "|Chicken Burger|";
               //orderList1 = "B1"; 
               //send("Chicken Burger, ");
               orderListFinal = orderListFinal + orderList1;
                
              }



              if (p.x > 470 && p.x < 570 && p.y > 420 && p.y < 705 && p.z > MINPRESSURE && p.z < MAXPRESSURE)// Beef Burger
              {
                tft.fillRoundRect(30, 90, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);
                tft.setCursor(37, 97);
                tft.print(" Beef Burger");
                delay(70);

                bill = 150;
                totalBill = totalBill + bill;

               orderCheckout = "  Beef Burger\t\t\t\t\t\t\t150\n" + orderCheckout;
               //orderList1 = "|Beef Burger|";
               orderList1 = "B2";

               orderListFinal = orderListFinal + orderList1;
                
              }

 
              
              if (p.x > 320 && p.x < 400 && p.y > 360 && p.y < 700 && p.z > MINPRESSURE && p.z < MAXPRESSURE)// Cheese Burger
              {
                tft.fillRoundRect(30, 140, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);
                tft.setCursor(37, 147);
                tft.print(" Cheese Burger");
                delay(70);

                bill = 100;
                totalBill = totalBill + bill;

                

               orderCheckout = "  Cheese Burger\t\t\t\t\t100\n" + orderCheckout;
               //orderList1 = "|Cheese Burger|";
               orderList1 = "B3";
                
               orderListFinal = orderListFinal + orderList1;
                
              }

 
        
              if(p.x > 170 && p.x < 240 && p.y > 620 && p.y < 860 && p.z > MINPRESSURE && p.z < MAXPRESSURE) // Checkout
              {
                tft.fillRoundRect(170, 190, 130, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(170, 190, 130, 40, 8, RED);
                tft.drawRoundRect(170, 190, 130, 40, 8, BLACK);
                tft.setCursor(177, 197);
                tft.print(" Checkout");
                delay(70);
                drawCheckout();
                delay(500);
                currentpage = 'C';
               
              }
      }

  if (currentpage == 'P')//Pizza
  {
     
     if( p.x > 160 && p.x < 275 && p.y > 275 && p.y < 520 && p.z > MINPRESSURE && p.z < MAXPRESSURE)
     {
            tft.fillRoundRect(30, 190, 120, 40, 8, WHITE);
            delay(70);

            tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);
            tft.drawRoundRect(30, 190, 120, 40, 8, BLACK);
            tft.setCursor(37, 197);
            tft.print(" Home");
            delay(70);  

            drawHome();
            currentpage = 'H';
            delay(1000);
     }

     if(p.x > 647 && p.x < 725 && p.y > 300 && p.y < 460 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Supreme Pizza
     {
      
                tft.fillRoundRect(30, 40, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);
                tft.setCursor(37, 47);
                tft.print(" Supreme Pizza");
                delay(70);

                bill = 450;
                totalBill = totalBill + bill;

                orderCheckout = "  Supreme Pizza\t\t\t\t\t450\n" + orderCheckout;
                
         
                //orderList1 = "|Supreme Pizza|";
                orderList1 = "P1";
                orderListFinal = orderListFinal + orderList1;
                
                }

  
        
        if(p.x > 470 && p.x < 560 && p.y > 430 && p.y < 700 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Deluxe Pizza
              {
      
                tft.fillRoundRect(30, 90, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);
                tft.setCursor(37, 97);
                tft.print(" Deluxe Pizza");
                delay(70);

                bill = 490;
                totalBill = totalBill + bill;

                orderCheckout = "  Deluxe Pizza\t\t\t\t\t\t490\n" + orderCheckout;
                //orderList1 = "|Deluxe Pizza|";
                orderList1 = "P2";

                
                orderListFinal = orderListFinal + orderList1;           
                
              }



        
        if(p.x > 312 && p.x < 390 && p.y > 466 && p.y < 700 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Cheese Pizza
                {
      
                tft.fillRoundRect(30, 140, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);
                tft.setCursor(37, 147);
                tft.print(" Cheese Pizza");
                delay(70);

                bill = 350;
                totalBill = totalBill + bill;

                orderCheckout = "  Cheese Pizza\t\t\t\t\t350\n" + orderCheckout;
                //orderList1 = "|Cheese Pizza|";
                orderList1 = "P3";
                orderListFinal = orderListFinal + orderList1;
                
     }

     
     

     if(p.x >166 && p.x <230 && p.y > 645 && p.y <857 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Checkout
     {
                tft.fillRoundRect(170, 190, 130, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(170, 190, 130, 40, 8, RED);
                tft.drawRoundRect(170, 190, 130, 40, 8, BLACK);
                tft.setCursor(177, 197);
                tft.print(" Checkout");
                delay(70);
                drawCheckout();
                
                delay(1000);
                currentpage = 'C';
               
     }


     

      
  }//Pizza Menu Block

  if(currentpage == 'D')// Hotdogs Pag
  {
    if(p.x > 640 && p.x < 780 && p.y > 365 && p.y < 690 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Chili Dog
    {
       tft.fillRoundRect(30, 40, 200, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE);
       tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);
       tft.setCursor(37, 47);
       tft.print(" Chili Dog");
       delay(70);

       bill = 80;
       totalBill = totalBill + bill;

       orderCheckout = "  Chili Dog\t\t\t\t\t\t\t\t\t\t80\n" + orderCheckout;
       //orderList1 = "|Chili Dog|";
       orderList1 = "H1";
       orderListFinal = orderListFinal + orderList1;
       
    }

    


    
     if(p.x > 474 && p.x < 560 && p.y > 360 && p.y < 690 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Hot Onion Dog
     {
      
                tft.fillRoundRect(30, 90, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);
                tft.setCursor(37, 97);
                tft.print(" Hot Onion Dog");
                delay(70);

                bill = 100;
                totalBill = totalBill + bill;

                orderCheckout = "  Hot Onion Dog\t\t\t\t\t100\n" + orderCheckout;
                //orderList1 = "|Hot Onion Dog|";
                orderList1 = "H2";           
                orderListFinal = orderListFinal + orderList1;
     }

 
     
       if(p.x > 312 && p.x < 390 && p.y > 466 && p.y < 700 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Cheese Dog
             {
      
                tft.fillRoundRect(30, 140, 200, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);
                tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);
                tft.setCursor(37, 147);
                tft.print(" Cheese Dog");
                delay(70);

                bill = 110;
                totalBill = totalBill + bill;

                orderCheckout = "  Cheese Dog\t\t\t\t\t\t\t\t110\n" + orderCheckout;
                //orderList1 = "|Cheese Dog|";
                orderList1 = "H3";
                orderListFinal = orderListFinal + orderList1;
                
             }

 
     

         if(p.x > 150 && p.x < 230 && p.y > 285 && p.y < 500 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Home Button
            {
                
               //Serial.println("Home");
               tft.fillRoundRect(30, 190, 120, 40, 8, WHITE);
               delay(70);

               tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);
               tft.drawRoundRect(30, 190, 120, 40, 8, BLACK);
               tft.setCursor(37, 197);
               tft.print("  Home");
               delay(70);
                
               drawHome();
               currentpage = 'H';
             }

               if(p.x >166 && p.x <230 && p.y > 645 && p.y <857 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Checkout
     {
                tft.fillRoundRect(170, 190, 130, 40, 8, WHITE);
                delay(70);

                tft.fillRoundRect(170, 190, 130, 40, 8, RED);
                tft.drawRoundRect(170, 190, 130, 40, 8, BLACK);
                tft.setCursor(177, 197);
                tft.print(" Checkout");
                delay(70);
                drawCheckout();
                
                delay(1000);
                currentpage = 'C';
               
     }

  }

if(currentpage == 'R')//Drinks Menu
{
    if(p.x > 288 && p.x < 360 && p.y > 450 && p.y < 630 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Home
    {
       tft.fillRoundRect(100, 150, 100, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(100, 150, 100, 40, 8, BLUE_GRAY);
       tft.drawRoundRect(100, 150, 100, 40, 8, BLACK);
       tft.setCursor(107, 157);
       tft.print("  Home");
       delay(200);

       
       drawHome();
       currentpage = 'H';
  
   }

    if(p.x > 520 && p.x < 704 && p.y > 253 && p.y < 480 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Cold Drinks
     {
      
                tft.fillRoundRect(20, 40, 120, 80, 8, WHITE);
                delay(70);

                tft.fillRoundRect(20, 40, 120, 80, 8, ORANGE);
                tft.drawRoundRect(20, 40, 120, 80, 8, BLACK);
                tft.setCursor(27, 47);
                tft.print("  Cold");
                tft.setCursor(27,67);
                tft.print(" Drinks");
                tft.setCursor(27,87);
                tft.print("   20");
                delay(70);

                bill = 20;
                totalBill = totalBill + bill;

                orderCheckout = "  Cold Drink\t\t\t\t\t\t\t\t\t20\n" + orderCheckout;
                //orderList1 = "|Cold Drink|";
                orderList1 = "D1";
                orderListFinal = orderListFinal + orderList1;
                
     }

 

     

     if(p.x > 530 && p.x < 705 && p.y > 640 && p.y < 860 && p.z > MINPRESSURE && p.z < MAXPRESSURE) //Water
     {
      
                tft.fillRoundRect(180, 40, 120, 80, 8, WHITE);
                delay(70);

                tft.fillRoundRect(180,40, 120, 80, 8, ORANGE);
                tft.drawRoundRect(180, 40, 120, 80, 8, BLACK);
                tft.setCursor(187, 47);
                tft.print("  Water");
                tft.setCursor(187, 87);
                tft.print("   15");
                delay(70);

                bill = 15;
                totalBill = totalBill + bill;

                orderCheckout = "  Water\t\t\t\t\t\t\t\t\t\t\t\t\t\t15\n" + orderCheckout;
                //orderList1 = "|Water|";
                orderList1 = "D2";
                orderListFinal = orderListFinal + orderList1;
                
     }

   
   
}
  
  if(currentpage == 'W')//Help Menu
  {
      if(p.x > 288 && p.x < 360 && p.y > 450 && p.y < 630 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Home
    {
       tft.fillRoundRect(100, 150, 100, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(100, 150, 100, 40, 8, BLUE_GRAY);
       tft.drawRoundRect(100, 150, 100, 40, 8, BLACK);
       tft.setCursor(107, 157);
       tft.print(" Home");
       delay(200);

       
       drawHome();
       currentpage = 'H';
  
   }
  }
  
  if(currentpage == 'C') // Checkout page
  {
  
    if(p.x > 160 && p.x < 240 && p.y > 280 && p.y < 500 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Home
    {
       tft.fillRoundRect(30, 190, 100, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(30, 190, 100, 40, 8, BLUE_GRAY);
       tft.drawRoundRect(30, 190, 100, 40, 8, BLACK);
       tft.setCursor(37, 197);
       tft.print("  Home");
       delay(70);

       
       drawHome();
       currentpage = 'H';
       delay(70);    
    } 
    if(p.x > 160 && p.x < 230 && p.y > 530 && p.y < 720 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Pay
    {
       tft.fillRoundRect(140, 190, 100, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(140, 190, 100, 40, 8, RED);
       tft.drawRoundRect(140, 190, 100, 40, 8, BLACK);
       tft.setCursor(147, 197);
       tft.print("  Pay");
       delay(200);

       
       drawQR();
       currentpage = 'Q';
  
   }
  }
 
  if (currentpage == 'Q') //QR Code Page
  {
      
   if(p.x > 280 && p.x < 350 && p.y > 270 && p.y < 365 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Home
   {
       tft.fillRoundRect(30, 150, 80, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(30, 150, 80, 40, 8, RED);
       tft.drawRoundRect(30, 150, 80, 40, 8, BLACK);
       tft.setCursor(37, 157);
       tft.setTextColor(WHITE);
       tft.setTextSize(2);
       
       tft.print(" Home");
       delay(200);
 
       
       bill = 0;
       totalBill = 0;
       orderCheckout = "";
       drawHome();
       currentpage = 'H';
      
   }     
   if(p.x > 285 && p.x < 356 && p.y > 520 && p.y < 650 && p.z > MINPRESSURE && p.z < MAXPRESSURE)//Paid
   {
       tft.fillRoundRect(130, 150, 80, 40, 8, WHITE);
       delay(70);

       tft.fillRoundRect(130, 150, 80, 40, 8, LIME_GREEN);
       tft.drawRoundRect(130, 150, 80, 40, 8, BLACK);
       tft.setCursor(137, 157);
       tft.setTextColor(WHITE);
       tft.setTextSize(2);
       
       tft.print(" PAID");
       delay(70);


       const char *empty = "          ";
       rf_driver.send((uint8_t *)empty, strlen(empty));
       rf_driver.waitPacketSent();
       delay(200);

       //conv_orderListFinal = orderListFinal.c_str();
       const char *msg = orderListFinal.c_str();
       rf_driver.send((uint8_t *)msg, strlen(msg));
       rf_driver.waitPacketSent();
       delay(200);
       

       
       
       orderList1 = "";
       orderListFinal = "";
       
       bill = 0;
       totalBill = 0;
       orderCheckout = "";
       drawHome();
       currentpage = 'H';

       
      
   }
   
   
   
  }//QR Block Ends

        
 

}// void loop ends here


  void drawBurgers()
  {
  tft.fillScreen(WHITE);
  tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE); //Chicken Burger
  tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE); //Beef Burger
  tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);   //Cheese Burger
  tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);    // Border
  tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);   // Home
  tft.drawRoundRect(30, 190, 120, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 40, 60, 40, 8, BLUE);   //Chiken Burger Price
  tft.drawRoundRect(240, 40, 60, 40, 8, BLACK);  //Border
  tft.fillRoundRect(240, 90, 60, 40, 8, BLUE); // //Beef Burger Price
  tft.drawRoundRect(240, 90, 60, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 140, 60, 40, 8, BLUE);// Cheese Burger price
  tft.drawRoundRect(240, 140, 60, 40, 8, BLACK);// Border
  tft.fillRoundRect(170, 190, 130, 40, 8, RED);// Checkout
  tft.drawRoundRect(170, 190, 130, 40, 8, BLACK); // Checkout
  
 
  tft.setCursor(60, 10);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("   Burger Menu");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(37, 47);
  tft.print(" Chicken Burger");
  tft.setCursor(37, 97);
  tft.print(" Beef Burger");
  tft.setCursor(37, 147);
  tft.print(" Cheese Burger");
  tft.setCursor(37, 197);
  tft.print("  Home");
  tft.setCursor(247, 47);
  tft.print("120");
  tft.setCursor(247, 97);
  tft.print("150");
  
  tft.setCursor(247, 147);
  tft.print("100");
  tft.setCursor(177, 197);
  tft.print(" Checkout");
  delay(1000);
  }


  void drawPizza()
  {
 
  tft.fillScreen(WHITE);
  tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE); //Supreme Pizza
  tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE); //Deluxe Pizza
  tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);   //Cheese Pizza
  tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);    // Border
  tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);   // Home
  tft.drawRoundRect(30, 190, 120, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 40, 60, 40, 8, BLUE);   //Supreme Pizza Price
  tft.drawRoundRect(240, 40, 60, 40, 8, BLACK);  //Border
  tft.fillRoundRect(240, 90, 60, 40, 8, BLUE); // //Deluxe Pizza Price
  tft.drawRoundRect(240, 90, 60, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 140, 60, 40, 8, BLUE);// Cheese Pizza price
  tft.drawRoundRect(240, 140, 60, 40, 8, BLACK);// Border
  tft.fillRoundRect(180, 190, 120, 40, 8, RED);// Checkout
  tft.drawRoundRect(180, 190, 120, 40, 8, BLACK); //
  
 
  tft.setCursor(60, 10);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.print("   Pizza Menu");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(37, 47);
  tft.print(" Supreme Pizza");
  tft.setCursor(37, 97);
  tft.print(" Deluxe Pizza");
  tft.setCursor(37, 147);
  tft.print(" Cheese Pizza");
  tft.setCursor(37, 197);
  tft.print("  Home");
  tft.setCursor(247, 47);
  tft.print("450");
  tft.setCursor(247, 97);
  tft.print("490");
  
  tft.setCursor(247, 147);
  tft.print("350");
  tft.setCursor(187, 197);
  tft.print(" Checkout");
  delay(500);
   
  }

  void drawHotDogs()
  {
  tft.fillScreen(WHITE);
  tft.fillRoundRect(30, 40, 200, 40, 8, ORANGE); //Supreme Pizza
  tft.drawRoundRect(30, 40, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 90, 200, 40, 8, ORANGE); //Deluxe Pizza
  tft.drawRoundRect(30, 90, 200, 40, 8, BLACK);  // Border
  tft.fillRoundRect(30, 140, 200, 40, 8, ORANGE);   //Cheese Pizza
  tft.drawRoundRect(30, 140, 200, 40, 8, BLACK);    // Border
  tft.fillRoundRect(30, 190, 120, 40, 8, BLUE_GRAY);   // Home
  tft.drawRoundRect(30, 190, 120, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 40, 60, 40, 8, BLUE);   //Supreme Pizza Price
  tft.drawRoundRect(240, 40, 60, 40, 8, BLACK);  //Border
  tft.fillRoundRect(240, 90, 60, 40, 8, BLUE); // //Deluxe Pizza Price
  tft.drawRoundRect(240, 90, 60, 40, 8, BLACK); //  Border
  tft.fillRoundRect(240, 140, 60, 40, 8, BLUE);// Cheese Pizza price
  tft.drawRoundRect(240, 140, 60, 40, 8, BLACK);// Border
  tft.fillRoundRect(180, 190, 120, 40, 8, RED);// Checkout
  tft.drawRoundRect(180, 190, 120, 40, 8, BLACK); //
  
 
  tft.setCursor(50, 10);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.print("   Hot Dogs Menu");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(37, 47);
  tft.print(" Chili Dog");
  tft.setCursor(37, 97);
  tft.print(" Hot Onion Dog");
  tft.setCursor(37, 147);
  tft.print(" Cheese Dog");
  tft.setCursor(37, 197);
  tft.print("  Home");
  tft.setCursor(247, 47);
  tft.print(" 80");
  tft.setCursor(247, 97);
  tft.print("100");
  
  tft.setCursor(247, 147);
  tft.print("110");
  tft.setCursor(187, 197);
  tft.print(" Checkout");
  delay(500);
  }

void drawDrinks()
{
  tft.fillScreen(WHITE);
  tft.fillRoundRect(20, 40, 120, 80, 8, ORANGE); //Cold Drinks
  tft.drawRoundRect(20, 40, 120, 80, 8, BLACK);  // Border
  tft.fillRoundRect(180,40, 120, 80, 8, ORANGE); // Water
  tft.drawRoundRect(180,40, 120, 80, 8, BLACK);
 
  tft.fillRoundRect(100, 150, 100, 40, 8, BLUE_GRAY);   // Home
  tft.drawRoundRect(100, 150, 100, 40, 8, BLACK); //  Border

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(27, 47);
  tft.print("  Cold");
  tft.setCursor(27,67);
  tft.print(" Drinks");
  tft.setCursor(27,87);
  tft.print("   20");

  tft.setCursor(187, 47);
  tft.print("  Water");
  tft.setCursor(187, 87);
  tft.print("   15");
  
  tft.setCursor(107, 157);
  tft.print("  Home");
}
void drawHelp()
{
  tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(30,40);
  tft.print("Someone from the staff");
  tft.setCursor(30,60);
  tft.print("will be here shortly.");
  tft.fillRoundRect(100, 150, 100, 40, 8, BLUE_GRAY);   // Home
  tft.drawRoundRect(100, 150, 100, 40, 8, BLACK); //  Border

  tft.setTextColor(WHITE);
  tft.setCursor(107, 157);
  tft.print(" Home");
  
  
}


  void drawCheckout()
  {
    tft.fillScreen(WHITE);
    tft.setCursor(90, 10);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.print(" Checkout");

    tft.setCursor(0,47);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.print(orderCheckout);
    tft.print("  ---------------------\n");
    tft.print("              Total " + String(totalBill));

   
   tft.fillRoundRect(140, 190, 100, 40, 8, RED); 
   tft.drawRoundRect(140, 190, 100, 40, 8, BLACK); 
   tft.fillRoundRect(30, 190, 100, 40, 8, BLUE_GRAY);  
   tft.drawRoundRect(30, 190, 100, 40, 8, BLACK);
  
   tft.setTextSize(2);
   tft.setTextColor(WHITE); 
   tft.setCursor(147,197);
   tft.print("  Pay");
   tft.setCursor(37,197);
   tft.print("  Home");
  } 


  void drawQR()
  {
      tft.fillScreen(WHITE);
      drawBitmap(230,30,qr,80,80,BLACK);
      tft.fillRoundRect(30, 150, 80, 40, 8, RED);   // Home
      tft.drawRoundRect(30, 150, 80, 40, 8, BLACK); //  Border
      tft.setCursor(37,157);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.print(" Home");

      tft.fillRoundRect(130,150,80,40,8,LIME_GREEN); //Paid
      tft.drawRoundRect(130,150,80,40,8, BLACK);
      tft.setCursor(137, 157);
      tft.print(" PAID");

      tft.setCursor(10,30);
      tft.setTextSize(2);
      tft.setTextColor(BLACK);
      tft.print("Scan the QR code");
      tft.setCursor(10,50);
      tft.print("Pay BDT "); 
      tft.setTextColor(RED);
      tft.print(String(totalBill));
      tft.setTextColor(BLACK);
      tft.setCursor(10,75);
      tft.print("After the payment");
      tft.setCursor(10,95);
      tft.print("is done, touch the");
      tft.setCursor(10,115);
      tft.print("'PAID' button.");
    
     
  }


  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
  {
    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j=0; j<h; j++)
    {
      for(i=0; i<w; i++) 
      {
        if(i & 7) byte <<=1;
        else      byte   = pgm_read_byte(bitmap + j * byteWidth + i /8);
        if(byte & 0x80) tft.drawPixel(x+i, y+j, color);
      }
    }
  }
/*
  void send (char *msg)
{
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  delay(1000);// Wait until the whole message is gone
}

*/
  
