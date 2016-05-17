/************************************************************************/
/* Uygulama Adi	:	5x7 Matris LED Kullanarak Kayan Yazi Uygulamasi	       */
/* Dosya Ad�	:	Kayan_Yazi				       */
/* Tarih		:	Mayis 2005						       */
/* Hedef ��lemci  :	MCS-51						       */
/*											       */
/************************************************************************/
/* K�t�phane Dosyasi */
#pragma large		// B�t�n de�i�ken ve sabitleri b�y�k boyutlu k�lar.
#include <89c51rd2.H>

#define satirtarama     P1      // Sat�rlar� Tarar.

// Tus Takimi S�tun Tanimlamalari
#define Sutun1          P2_0
#define Sutun2          P2_1
#define Sutun3          P2_2

// Tus Takimi Satir Tanimlamalari
#define SatA            P2_3
#define SatB            P2_4
#define SatC            P2_5
#define SatD            P2_6

/* Kaymali Kaydedici Uc Tanimlamalari */
#define reset           P3_0    // Shift Register'i Resetler.
#define clock           P3_1    // Shift Register'a (Clock)saat Palsi G�nderir.
#define datas           P3_2    // Shift Register'a Data'y� G�nderir.

#define sil             P3_3    // Yaz�lacak Verileri Siler.
#define rst             P3_4    // ��lemciyi resetler.
 
// De�i�kenlerin ilk de�erleriyle birlikte tan�mlamas�.
 int    enter=1,
        dizidekikarakter=0,
        tus=0,
        tekrar[10]={0,0,0,0,0,0,0,0,0,0},               
        sifirmi=0,
        bak=0,
        sifirla=0,
        satirelemani=0,
        yazilacakdizi[20][6],
        krkt=0,
        kar=0,
        esasyaz[240],
        yazmayitekraret=0,
        satirsay=0,
        satelm=0,
        gecsatirtarama=0x01,
        bekle=0,
        karaktersayisi=29;
 code unsigned int
 karakterler[50][6]={{0x00,0x00,0x00,0x00,0x00,0x00},    //  Bosluk
                     {0x00,0x00,0x60,0x60,0x00,0x00},    //  .
					 {0x00,0x00,0x58,0x38,0x00,0x00},    //  ,
					 {0x00,0x00,0x66,0x66,0x00,0x00},    //  :
					 {0x00,0x3E,0x51,0x49,0x45,0x3E},    //  0
					 
					 {0x00,0x00,0x42,0x7F,0x40,0x00},    //  1
					 {0x00,0x08,0x08,0x3E,0x08,0x08},    //  +
                     {0x00,0x00,0x08,0x08,0x08,0x00},    //  -
                     {0x00,0x22,0x14,0x7F,0x14,0x22},    //  *
                     {0x00,0x20,0x10,0x08,0x04,0x02},    //  /

                     {0x00,0x42,0x61,0x51,0x49,0x46},    //  2
                     {0x00,0x7E,0x09,0x09,0x09,0x7E},    //  A
                     {0x00,0x7F,0x49,0x49,0x49,0x36},    //  B
                     {0x00,0x3E,0x41,0x41,0x41,0x22},    //  C
                     {0x00,0x1E,0x21,0x61,0x21,0x12},    //  �

                     {0x00,0x22,0x49,0x49,0x49,0x36},    //  3
                     {0x00,0x7F,0x41,0x41,0x41,0x3E},    //  D
                     {0x00,0x7F,0x49,0x49,0x49,0x41},    //  E
                     {0x00,0x7F,0x09,0x09,0x09,0x01},    //  F
                     {0x00,0x3E,0x41,0x49,0x49,0x7A},    //  G

                     {0x00,0x0F,0x08,0x08,0x08,0x7F},    //  4
                     {0x00,0x3C,0x43,0x53,0x53,0x74},    //  �
                     {0x00,0x7F,0x08,0x08,0x08,0x7F},    //  H
                     {0x00,0x00,0x41,0x7F,0x41,0x00},    //  I
                     {0x00,0x00,0x42,0x7F,0x42,0x00},    //  �

                     {0x00,0x4F,0x49,0x49,0x49,0x31},    //  5
                     {0x00,0x20,0x40,0x40,0x40,0x3F},    //  J
                     {0x00,0x7F,0x08,0x14,0x22,0x41},    //  K
                     {0x00,0x7F,0x40,0x40,0x40,0x40},    //  L
                     {0x00,0x7F,0x02,0x04,0x02,0x7F},    //  M

                     {0x00,0x3E,0x49,0x49,0x49,0x32},    //  6
                     {0x00,0x7F,0x02,0x04,0x08,0x7F},    //  N
                     {0x00,0x3E,0x41,0x41,0x41,0x3E},    //  O
                     {0x00,0x3C,0x43,0x42,0x43,0x3C},    //  �
                     {0x00,0x7F,0x09,0x09,0x09,0x06},    //  P

                     {0x00,0x01,0x71,0x09,0x05,0x03},    //  7
                     {0x00,0x3E,0x41,0x51,0x61,0x7E},    //  Q
                     {0x00,0x7F,0x09,0x19,0x29,0x46},    //  R                   
					 {0x00,0x26,0x49,0x49,0x49,0x32},    //  S
                     {0x00,0x02,0x15,0x55,0x15,0x08},    //  �

                     {0x00,0x36,0x49,0x49,0x49,0x36},    //  8
                     {0x00,0x01,0x01,0x7F,0x01,0x01},    //  T
                     {0x00,0x3F,0x40,0x40,0x40,0x3F},    //  U
                     {0x00,0x3C,0x41,0x40,0x41,0x3C},    //  �
                     {0x00,0x1F,0x20,0x40,0x20,0x1F},    //  V

                     {0x00,0x06,0x49,0x49,0x49,0x3E},    //  9
                     {0x00,0x3F,0x40,0x30,0x40,0x3F},    //  W
                     {0x00,0x63,0x14,0x08,0x14,0x63},    //  X
                     {0x00,0x03,0x04,0x78,0x04,0x03},    //  Y
                     {0x00,0x61,0x51,0x49,0x45,0x43}};   //  Z


//////////////////////////////////////////////////////////////////////
// 					Tus Takimini Tarama Fonksiyonu				   //
//////////////////////////////////////////////////////////////////////


tusoku() large // Metin Giri�i (Tu�lar� Okur.)
{
 SatA=0;            // satir-a'n�n taran�r.
 while(!SatA)
    {// Tus 1
     if (Sutun1==0) {tus=1; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun1){;}}
    // Tus 2
     if (Sutun2==0) {tus=2; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun2){;}}
    // Tus 3
     if (Sutun3==0) {tus=3; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun3){;}}
     SatA=1;
    }
 SatB=0;            // satir-b taran�r.
 while(!SatB)
    {// Tus 4
     if (Sutun1==0) {tus=4; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun1){;}}
    // Tus 5
     if (Sutun2==0) {tus=5; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun2){;}}
    // Tus 6
     if (Sutun3==0) {tus=6; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun3){;}}
     SatB=1;
    }
 SatC=0;            // satir-c taran�r.
 while(!SatC)
    {// Tus 7
     if (Sutun1==0) {tus=7; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun1){;}}
    // Tus 8
     if (Sutun2==0) {tus=8; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun2){;}}
    // Tus 9
     if (Sutun3==0) {tus=9; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun3){;}}
     SatC=1;
    }
 SatD=0;            // satir-d taran�r.
 while(!SatD)
    {// * Tu�u
     if (Sutun1==0) {
                     dizidekikarakter++;// Bir sonraki karaktere ge�er.
                     tus=0;				// Tu� bilgisi s�f�rlan�r.
                     while(!Sutun1){;}
                     for(sifirla=0;sifirla<10;sifirla++)
                        tekrar[sifirla]=0;  //"tekrar" dizisi s�f�rlan�r.
                    }                       
    // Tus 0
     if (Sutun2==0) {tus=0; tekrar[tus]++; tekrar[tus]=tekrar[tus]%5; while(!Sutun2){;}}
    // Yaz� okumay� bitirir.
     if (Sutun3==0) {enter=0;while(!Sutun3){;}}
     SatD=1;
    }
}
//////////////////////////////////////////////////////////////////////
// 		Karakterleri Yazdirilacak Diziye Ekleyen Fonksiyon		   //
//////////////////////////////////////////////////////////////////////


yazial() large // Karakteri "yazilacakdizi" dizisine ekler.
{
 for (satirelemani=0;satirelemani<6;satirelemani++)
     yazilacakdizi[  dizidekikarakter   ][satirelemani]
    =karakterler  [ (tus*5)+tekrar[tus] ][satirelemani];
}

//////////////////////////////////////////////////////////////////////
// 			Yazilacak Karakteri G�r�nt�leme Fonksiyonu				   //
//////////////////////////////////////////////////////////////////////

goster() large 
{// Harflerin g�r�nebilmesi i�in yazmay� tekrar eder.
 for(yazmayitekraret=0;yazmayitekraret<2;yazmayitekraret++)
    for (satirsay=0;satirsay<7;satirsay++)  // Herbir sat�r� tek tek yazmak i�in.
        {
         reset=0;   // Shift Register resetlenir.
         reset=1;
         // "yazilacakdizi" dizisindeki kayd�r�lacak yaz�y� yazdirmak i�in.
         for(satelm=0;satelm<(dizidekikarakter+1)*6;satelm++)
            {// "yazilacakdizi" dizisinden, yaz�lacak sat�r bilgisi se�ilir.
             datas=yazilacakdizi[krkt][satelm]&gecsatirtarama;
             clock=0;   // Herbir Clock'ta haz�r olan s�tun eleman�,
             clock=1;   // Shift Register'da kayar.
            }
         // gecsatirtarama de�i�keni ka��nc� sat�r�n aktif olaca��n� ifade eder.
         satirtarama=gecsatirtarama;
         // Herbir yazmada ledlerin yand���n�n g�r�nmesi i�in gecikme.
         for(bekle=0;bekle<1;bekle++) {;}
         // Bir sonraki sat�r�n bilgisi haz�rlan�rken g�r�nmemesi i�in sat�r silinir.
         satirtarama=0x00;
         gecsatirtarama=gecsatirtarama<<1;  // Bir sonraki sat�ra ge�ilir.
         // 7 Sat�r tarand�ktan sonra, taramay� ba�a d�nd�r�r.
         if (gecsatirtarama==0x80) gecsatirtarama=0x01;
         if (sil==0)
            {// En son al�nm�� karakteri "yazilacakdizi" dizisinden siler.
             while (!sil) {;}
             for(kar=0;kar<6;kar++) yazilacakdizi[dizidekikarakter][kar]=0;
			 // "dizidekikarakter" dizisi 1 eksiltilir.
             if (dizidekikarakter!=0) dizidekikarakter--;
			 tus=0;		// Tu� bilgisi s�f�rlan�r.
            }
        }
 satirtarama=0x00;	// Ekran her tarama sonunda temizlenir.
}

//////////////////////////////////////////////////////////////////////
// 	   Matris LEDlere G�nderilecek Diziyi Olusturan Fonksiyon		//
//////////////////////////////////////////////////////////////////////

tekbdizi() large // "esasyaz" dizisine 10 karakterlik bo�luk b�rakacak �ekilde
		 //"yazilacakdizi" dizisinden "esasyaz" dizisine karakterleri aktar�r.
{
 for (krkt=9;krkt<karaktersayisi;krkt++)
    for (kar=0;kar<6;kar++)
        esasyaz[(krkt*6)+kar]=yazilacakdizi[krkt-9][kar];
 // "esasyaz" dizisinin kullan�lmam�� yerlerine 0 atar.
 for (kar=(krkt*6)+kar;kar<240;kar++) esasyaz[kar]=0;
}

//////////////////////////////////////////////////////////////////////
// 					Yaziyi Kaydirma Fonksiyonu				   //
//////////////////////////////////////////////////////////////////////

kaydir() large // Metni Yazar ve Kayd�r�r.
{
 while (1)
    {
     for (krkt=0;krkt<60+(dizidekikarakter*6);krkt++)
        // Harflerin g�r�nebilmesi i�in yazmay� tekrar eder.
        for(yazmayitekraret=0;yazmayitekraret<10;yazmayitekraret++)
            for (satirsay=0;satirsay<7;satirsay++)  // Herbir sat�r� tek tek yazmak i�in.
                {// Shift Register resetlenir.
                 reset=0;
                 reset=1;
                 // esasyaz dizisindeki 60 s�tun eleman�n� yazdirmak i�in.
                 for(satelm=0;satelm<60;satelm++)
                    {// esasyaz dizisinden, yaz�lacak sat�r bilgisi se�ilir.
                     datas=esasyaz[krkt+satelm]&gecsatirtarama;
                     clock=0;   // Herbir Clock'ta haz�r olan s�tun eleman�,
                     clock=1;   // Shift Register'da kayar.
                    }
                 // gecsatirtarama de�i�keni ka��nc� sat�r�n aktif olaca��n� ifade eder.
                 satirtarama=gecsatirtarama;
                 // Herbir yazmada ledlerin yand���n�n g�r�nmesi i�in gecikme.
                 for(bekle=0;bekle<1;bekle++) {;}
                 // Bir sonraki sat�r�n bilgisi haz�rlan�rken g�r�nmemesi i�in sat�r silinir.
                 satirtarama=0x00;
                 gecsatirtarama=gecsatirtarama<<1;  // Bir sonraki sat�ra ge�ilir.
                 // 7 Sat�r tarand�ktan sonra, taramay� ba�a d�nd�r�r.
                 if (gecsatirtarama==0x80) gecsatirtarama=0x01;
                 if (sil==0)
                    {// Diziler Silinir.
                     while (!sil) {;}
                     for(sifirla=0;sifirla<10;sifirla++) tekrar[sifirla]=0;             
                     for(sifirla=0;sifirla<20;sifirla++)
                        for(kar=0;kar<6;kar++) yazilacakdizi[sifirla][kar]=0;
                     for(sifirla=0;sifirla<192;sifirla++) esasyaz[sifirla]=0;
                     rst=0; //Program Resetlenir.
                     rst=1;
                    }
                }
    }
}

//////////////////////////////////////////////////////////////////////
// 					ANA FONKSIYON				   //
//////////////////////////////////////////////////////////////////////

main() large 
{ 
 rst=1;
 while(enter)
    {
     if (dizidekikarakter==20) break;
     tusoku();
     yazial();
     goster();
    }
 tekbdizi();
 kaydir();
}