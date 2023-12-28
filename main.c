#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_KULLANICILAR 100
#define KULLANICI_ADI_UZUNLUK 20
#define SIFRE_UZUNLUK 20

typedef struct
{
	char kullaniciAdi[KULLANICI_ADI_UZUNLUK];
	char sifre[SIFRE_UZUNLUK];
} Kullanici;

typedef struct Bolum
{
	int bolumID;
	char bolumAd[30];
} bolum;

typedef struct Kisi             //Öğrenci yapısı tanımlanmaktadır.
{
	int numara;
	char tc[20], adSoyad[20], dTarih[20], dYeri[20], cinsiyet, adres[30], tel[20], ePosta[30];
	int askerlikDurumu, bolumID, durum; // durum = 0 ise silinmis, 1 = aktif, 2 = mezun
} ogr;

typedef struct Ders            //Ders yapısı tanımlanmaktadır. 
{
	int dersID;
	int bolumID;
	int akts;
	char dersAd[30];
	int ogrGorevlisiID;

} ders;

typedef struct Notlar           //Notlar yapısı tanımlanmaktadır.
{
	int notID;
	int dersID;
	int bolumID;

	int ogrGorevlisiID;
	int ogrenciID;
	float puan;
} notlar;

void bolumListele();

void dataKlasoru()            //Data klasörü oluşturma fonksiyonu
{
	const char *folderPath = "data";

	if (CreateDirectory(folderPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		// printf("Folder created successfully or already exists: %s\n", folderPath);
	}
	else
	{
		printf("Dizin hatası: %s\n", folderPath);
	}
}

void ogrenciEkle()            //Sisteme öğrenci ekleme fonksiyonu.
{
	system("cls");
	printf("Ogrenci ekleme islemi... \n\n");

	ogr o1;

	FILE *numPtr = fopen("./data/ogrenciNumaralari.dat", "a+b");       //Arka planda numara üretir.
	int numara = 0;
	while (fread(&numara, sizeof(int), 1, numPtr))
	{
	}
	numara += 1;

	o1.numara = numara;
	fwrite(&numara, sizeof(int), 1, numPtr);        //Öğrencinin bilgileri alındı.
	fclose(numPtr);
	printf("TC             : ");
	scanf(" %[^\n]s", o1.tc);
	printf("Ad-Soyad       : ");
	scanf(" %[^\n]s", o1.adSoyad);
	printf("Dogum Tarih    : ");
	scanf(" %[^\n]s", o1.dTarih);
	printf("Dogum Yeri     : ");
	scanf(" %[^\n]s", o1.dYeri);
	printf("Cinsiyet       : ");
	scanf(" %[^\n]s", &o1.cinsiyet);
	printf("Adres          : ");
	scanf(" %[^\n]s", o1.adres);
	printf("Telefon        : ");
	scanf(" %[^\n]s", o1.tel);
	printf("E-Posta        : ");
	scanf(" %[^\n]s", o1.ePosta);
	if (o1.cinsiyet == 'e' || o1.cinsiyet == 'E')
	{
		printf("Askerlik Durumu\n");
		printf("[Yapmadi (0), Yapti (1), Muaf(2)]  : ");
		scanf("%d", &o1.askerlikDurumu);                     //Kişi eğer kadınsa askerlik sorulmadı.
	}
	o1.askerlikDurumu = 3;
	fclose(numPtr);

	bolumListele();

	printf("Bolum Numarasi : ");
	scanf("%d", &o1.bolumID);
	o1.durum = 1;

	FILE *ptr = fopen("./data/ogrenciler.dat", "a+b");
	fwrite(&o1, sizeof(ogr), 1, ptr);                           //Oluşturulan numarayı kaydettik ve kapattık.
	fclose(ptr);

	printf("%d numarali ogrenci kaydi tamam \n", o1.numara);
}

void ogrenciSil()
{
	system("cls");
	printf("Ogrenci silme islemi... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");        //Okuma ve yazma formatın'da pointer oluşturduk.

	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara && o1.durum == 1)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali aktif ogrenci bulunamadi \n", numara);
	else
	{
		rewind(ptr);
		fseek(ptr, (sayac) * sizeof(ogr), 0);
		o1.durum = 0;                                    //Durum 0 ise öğrencinin kaydı silinmiştir ve ya dondurulmuştur.
		fwrite(&o1, sizeof(ogr), 1, ptr);
		printf("%d numarali ogrenci kaydi silindi \n", numara);
	}

	fclose(ptr);
}

void ogrenciAra()
{
	system("cls");
	printf("Ogrenci arama islemi... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");

	printf("Numara : ");                                //Aranıcak olan öğrencinin numarasını sorgulama.
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogrenci bulunamadi \n", numara);
	else
	{

		printf("%d numarali ogrenci bilgileri \n", numara);        //Öğrencinin kayıtlı bilgileri burada gösterilir.
		printf("TC          : %s \n", o1.tc);
		printf("Ad-Soyad    : %s \n", o1.adSoyad);
		printf("Bolumu      : %d \n", o1.bolumID);
		printf("Durumu      : %d \n", o1.durum);
		printf("Adres       : %s \n", o1.adres);
		printf("Telefon     : %s \n", o1.tel);
	}
	fclose(ptr);
}

void ogrenciListele()
{
	system("cls");
	printf("Ogrenci listele islemi... \n\n");

	ogr o1;
	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");

	bolumListele();
	int bolumNo, sayac = 0;
	printf("Bolum No : ");
	scanf("%d", &bolumNo);

	printf("%-20s%-20s%-30s\n", "NUMARA", "TC", "AD-SOYAD");           //Öğrencinin bilgileri alındı.
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (bolumNo == o1.bolumID && o1.durum == 1)
		{
			printf("%-20d%-20s%-30s \n", o1.numara, o1.tc, o1.adSoyad);         //Hangi bölümün öğrencisi ise onu listeliyoruz.
			sayac++;
		}
	}
	fclose(ptr);
	printf("\nToplam ogrenci sayisi : %d \n", sayac);
}

void ogrenciBelgesi()
{
	system("cls");
	printf("Ogrenci belgesi islemleri... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");

	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogrenci bulunamadi \n", numara);
	else
	{
		if (o1.durum == 0)                                          //Öğrencinin aktif mi yoksa mezun bir öğrenci mi olduğunu gösterilmiştir.
		{
			printf("%d numarali ogrencin kaydi SILINMISTIR . bilgileri asagidadir. \n", numara);
			printf("TC          : %s \n", o1.tc);
			printf("Ad-Soyad    : %s \n", o1.adSoyad);
			printf("Dogum tarihi: %s \n", o1.dTarih);
			printf("Bolumu      : %d \n", o1.bolumID);
			printf("Durumu      : %d \n", o1.durum);
			printf("Adres       : %s \n", o1.adres);
			printf("Telefon     : %s \n", o1.tel);
		}

		else if (o1.durum == 1)                                    //Durum Eğer 1 ise aktif kayıtlı öğrencidir.
		{
			printf("%d numarali ogrenci AKTIF kayitli ogrencidir . bilgileri asagidadir. \n", numara);
			printf("TC          : %s \n", o1.tc);
			printf("Ad-Soyad    : %s \n", o1.adSoyad);
			printf("Dogum tarihi: %s \n", o1.dTarih);
			printf("Bolumu      : %d \n", o1.bolumID);
			printf("Durumu      : %d \n", o1.durum);
			printf("Adres       : %s \n", o1.adres);
			printf("Telefon     : %s \n", o1.tel);
		}
		else if (o1.durum == 2)
		{
			printf("%d numarali ogrenci Fakultemizden MEZUN olmustur . bilgileri asagidadir. \n", numara);
			printf("TC          : %s \n", o1.tc);
			printf("Ad-Soyad    : %s \n", o1.adSoyad);
			printf("Dogum tarihi: %s \n", o1.dTarih);
			printf("Bolumu      : %d \n", o1.bolumID);
			printf("Durumu      : %d \n", o1.durum);
			printf("Adres       : %s \n", o1.adres);
			printf("Telefon     : %s \n", o1.tel);
		}
	}
	fclose(ptr);
}
void ogrenciTranskript()
{
	system("cls");
	printf("Ogrenci transkript islemleri... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");

	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogrenci bulunamadi \n", numara);
	else
	{
		printf("%d numarali ogrenci BILGILERI \n\n", numara);
		printf("TC          : %s \n", o1.tc);
		printf("Ad-Soyad    : %s \n", o1.adSoyad);
		printf("Bolumu      : %d \n", o1.bolumID);
		printf("Durumu      : %d \n", o1.durum);
		printf("Adres       : %s \n", o1.adres);
		printf("Telefon     : %s \n", o1.tel);

		printf("NOT BILGILERI \n\n\n");

		notlar n1;

		FILE *ptr = fopen("./data/notlar.dat", "r+b");
		printf("%-20s%-20s%-20s\n", "DERS-ID", "OGRENCI-NO", "PUAN");
		while (fread(&n1, sizeof(notlar), 1, ptr))
		{

			if (numara == n1.ogrenciID)
				printf("%-20d%-20d%.2f\n", n1.dersID, n1.ogrenciID, n1.puan);
		}
		fclose(ptr);
	}
}
void ogrenciMezuniyet()
{
	system("cls");
	printf("Ogrenci mezuniyet islemi... \n\n");

	ogr o1;
	int numara, sonuc = 0, sayac = 0;

	FILE *ptr = fopen("./data/ogrenciler.dat", "r+b");
                                                                   //Yine numara sorgulama yapolmıştır.
	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara)
		{
			sonuc = 1;                                 //Öğrencinin durumu 0 ise silinmiştir 1 ise aktiftir.
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogrenci bulunamadi \n", numara);
	else
	{
		rewind(ptr);                                       //gösterilen akışda ki aktif dosya konumunu en başa alır.
		fseek(ptr, (sayac) * sizeof(ogr), 0);
		o1.durum = 2;
		fwrite(&o1, sizeof(ogr), 1, ptr);
		printf("%d numarali %s isimli ogrencinin mezuniyet islemi tamamlandi... \n", numara, o1.adSoyad);
	}
	fclose(ptr);
}

int ogrenciMenu()
{
	int secim;
	printf("\n\tOgrenci islemleri... \n\n");
	printf("\n\t1- Ogrenci Ekle  \n");
	printf("\n\t2- Ogrenci Sil  \n");
	printf("\n\t3- Ogrenci Ara  \n");
	printf("\n\t4- Ogrenci Listele  \n");
	printf("\n\t5- Ogrenci Belgesi  \n");
	printf("\n\t6- Ogrenci Transkript  \n");
	printf("\n\t7- Ogrenci Mezuniyet  \n");
	printf("\n\t0- Cikis \n");
	printf("\n\t- Seciminiz   :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void ogrenciIslemleri()
{
	int secim = ogrenciMenu();
	while (secim != 0)
	{
		switch (secim)
		{
		case 1:
			ogrenciEkle();
			break;
		case 2:
			ogrenciSil();
			break;
		case 3:
			ogrenciAra();
			break;
		case 4:
			ogrenciListele();
			break;
		case 5:
			ogrenciBelgesi();
			break;
		case 6:
			ogrenciTranskript();
			break;
		case 7:
			ogrenciMezuniyet();
			break;
		case 0:
			break;
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = ogrenciMenu();
	}
	printf("Ogrenci islemlerinden cikis yaptiniz ... \n");
}
void ogretimGorevlisiEkle()
{
	system("cls");
	printf("Ogretim gorevlisi ekleme islemi... \n\n");

	ogr o1;

	FILE *numPtr = fopen("./data/ogretimGorevlisiNumaralari.dat", "a+b");
	int numara = 0;
	while (fread(&numara, sizeof(int), 1, numPtr))
	{
	}
	numara += 1;

	o1.numara = numara;
	fwrite(&numara, sizeof(int), 1, numPtr);
	fclose(numPtr);
	printf("TC             : ");
	scanf(" %[^\n]s", o1.tc);
	printf("Ad-Soyad       : ");
	scanf(" %[^\n]s", o1.adSoyad);
	printf("Dogum Tarih    : ");
	scanf(" %[^\n]s", o1.dTarih);
	printf("Dogum Yeri     : ");
	scanf(" %[^\n]s", o1.dYeri);
	printf("Cinsiyet       : ");
	scanf(" %[^\n]s", &o1.cinsiyet);
	printf("Adres          : ");
	scanf(" %[^\n]s", o1.adres);
	printf("Telefon        : ");
	scanf(" %[^\n]s", o1.tel);
	printf("E-Posta        : ");
	scanf(" %[^\n]s", o1.ePosta);
	if (o1.cinsiyet == 'e' || o1.cinsiyet == 'E')
	{
		printf("Askerlik Durumu\n");
		printf("[Yapmadi (0), Yapti (1), Muaf(2)]  : ");
		scanf("%d", &o1.askerlikDurumu);
	}
	o1.askerlikDurumu = 3;
	fclose(numPtr);

	bolumListele();

	printf("Bolum Numarasi : ");
	scanf("%d", &o1.bolumID);
	o1.durum = 1;

	FILE *ptr = fopen("./data/ogretimGorevlileri.dat", "a+b");
	fwrite(&o1, sizeof(ogr), 1, ptr);
	fclose(ptr);

	printf("%d numarali ogretim gorevlisi kaydi tamam \n", o1.numara);
}

void ogretimGorevlisiSil()
{
	system("cls");
	printf("Ogretim gorevlisi silme islemi... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogretimGorevlileri.dat", "r+b");

	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara && o1.durum == 1)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogretim gorevlisi bulunamadi \n", numara);
	else
	{
		rewind(ptr);
		fseek(ptr, (sayac) * sizeof(ogr), 0);
		o1.durum = 0;
		fwrite(&o1, sizeof(ogr), 1, ptr);
		printf("%d numarali ogretim gorevlisinin kaydi silindi \n", numara);
	}
	fclose(ptr);
}

void ogretimGorevlisiAra()
{
	system("cls");
	printf("Ogretim gorevlisi arama islemi... \n\n");

	ogr o1;
	int numara, sayac = 0, sonuc = 0;

	FILE *ptr = fopen("./data/ogretimGorevlileri.dat", "r+b");

	printf("Numara : ");
	scanf("%d", &numara);
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)
	{
		if (numara == o1.numara)
		{
			sonuc = 1;
			break;
		}
		sayac++;
	}
	if (sonuc == 0)
		printf("%d numarali ogretim gorevlisi bulunamadi \n", numara);
	else
	{

		printf("%d numarali ogretim gorevlisi bilgileri \n", numara);
		printf("TC          : %s \n", o1.tc);
		printf("Ad-Soyad    : %s \n", o1.adSoyad);

		printf("Bolumu      : %d \n", o1.bolumID);
		printf("Durum       : %d \n", o1.durum);
		printf("Adres       : %s \n", o1.adres);
		printf("Telefon     : %s \n", o1.tel);
	}

	fclose(ptr);
}

void ogretimGorevlisiListele()
{
	system("cls");
	printf("Ogretim gorevlisi listele islemi... \n\n");

	ogr o1;
	FILE *ptr = fopen("./data/ogretimGorevlileri.dat", "r+b");
	int bolumNo, sayac = 0;

	printf("%-20s%-20s%-30s%-20s\n", "NUMARA", "TC", "AD-SOYAD", "BOLUMU");
	while (fread(&o1, sizeof(ogr), 1, ptr) == 1)                                        //Bilgiler yazdırılmıştır.
	{
		printf("%-20d%-20s%-30s%-20d\n", o1.numara, o1.tc, o1.adSoyad, o1.bolumID);
		sayac++;
	}
	fclose(ptr);
	printf("\nToplam ogretim gorevlisi sayisi : %d \n", sayac);
}

int ogretimGorevlisiMenu()
{
	int secim;
	printf("\n\tOgretim gorevlisi islemleri... \n\n");
	printf("\n\t1- Ogretim Gorevlisi Ekle  \n");
	printf("\n\t2- Ogretim Gorevlisi Sil  \n");
	printf("\n\t3- Ogretim Gorevlisi Ara  \n");
	printf("\n\t4- Ogretim Gorevlisi Listele  \n");

	printf("\n\t0- Cikis \n");
	printf("\n\t- Seciminiz   :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void ogretimGorevlisiIslemleri()
{
	int secim = ogretimGorevlisiMenu();
	while (secim != 0)
	{
		switch (secim)
		{
		case 1:
			ogretimGorevlisiEkle();
			break;
		case 2:
			ogretimGorevlisiSil();
			break;
		case 3:
			ogretimGorevlisiAra();
			break;
		case 4:
			ogretimGorevlisiListele();
			break;
		case 0:
			break;
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = ogretimGorevlisiMenu();
	}
	printf("Ogretim gorevlisi islemlerinden cikis yaptiniz ... \n");
}

void dersEkle()                              //Sisteme ders ekleme fonksiyonu
{
	system("cls");
	printf("Ders ekleme islemi... \n\n");
	ders d1;

	printf("Ders Adi : ");
	scanf(" %[^\n]s", d1.dersAd);
	printf("AKTS : ");
	scanf(" %d", &d1.akts);

	bolumListele();
	printf("Bolum Numarasi : ");
	scanf(" %d", &d1.bolumID);

	ogretimGorevlisiListele();

	printf("Ogretim gorevlisi numarasi : ");
	scanf(" %d", &d1.ogrGorevlisiID);

	FILE *numPtr = fopen("./data/dersNumaralari.dat", "a+b");           //Dersler ekleme ve okuma modunda açılmaktadır.
	int numara = 0;
	while (fread(&numara, sizeof(int), 1, numPtr))
	{
	}
	numara += 1;
	d1.dersID = numara;
	fwrite(&numara, sizeof(int), 1, numPtr);
	fclose(numPtr);

	FILE *ptr = fopen("./data/dersler.dat", "a+b");
	fwrite(&d1, sizeof(ders), 1, ptr);                        //Eklenen ders bilgileri dosyaya yazılmaktadır.
	fclose(ptr);
	printf("%d numarali ders kaydi tamam \n", numara);
}

void dersListele()                 //Öğrencinin kayıtlı olduğu derslerin listelendiği fonksiyon.
{
	system("cls");
	printf("Ders listesi ... \n\n");

	ders d1;

	FILE *ptr = fopen("./data/dersler.dat", "r+b");
	printf("%-20s%-20s%-30s%-20s\n", "BOLUM-ID", "DERS-ID", "DERS-ADI", "Ogr.NO");
	while (fread(&d1, sizeof(ders), 1, ptr))                                         //Girilen öğreci numarası, sistemde kayıtlı bir öğrenci numarası olana kadar döngü devam etmektedir.
	{
		printf("%-20d%-20d%-30s%-20d\n", d1.bolumID, d1.dersID, d1.dersAd, d1.ogrGorevlisiID);
	}
	fclose(ptr);
}

int dersMenu()
{
	int secim;
	printf("\n\tDers islemleri... \n\n");
	printf("\n\t1- Ders Ekle  \n");
	printf("\n\t2- Ders Listele \n");
	printf("\n\t0- Cikis \n");
	printf("\n\t1- Seciminiz   :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void dersIslemleri()
{
	int secim = dersMenu();     //Yukarıda ki ders menüsü islemi çağırılmıştır.
	while (secim != 0)
	{
		switch (secim)
		{
		case 1:
			dersEkle();
			break;
		case 2:
			dersListele();
			break;
		case 0:
			break;
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = dersMenu();
	}
	printf("Ders islemlerinden cikis yaptiniz ... \n");
}

void bolumEkle()
{
	system("cls");
	printf("Bolum ekleme islemi... \n\n");
	bolum b1;
	printf("Bolum Adi : ");
	scanf(" %[^\n]s", b1.bolumAd);

	FILE *numPtr = fopen("./data/bolumNumaralari.dat", "a+b");      //Bolüm ID tutmak için oluşturulan dosya.
	int numara = 0;
	while (fread(&numara, sizeof(int), 1, numPtr))
	{
	}
	numara += 1;
	b1.bolumID = numara;
	fwrite(&numara, sizeof(int), 1, numPtr);
	fclose(numPtr);

	FILE *ptr = fopen("./data/bolumler.dat", "a+b");
	fwrite(&b1, sizeof(bolum), 1, ptr);
	fclose(ptr);

	printf("%d numarali bolum kaydi tamam \n", numara);
}

void bolumListele()
{
	system("cls");
	printf("Bolumler listesi \n\n");
	bolum b1;

	FILE *Ptr = fopen("./data/bolumler.dat", "rb");
	if (Ptr == NULL)
	{
		printf("Bolum dosyasi acilamadi! \n");
		return;
	}

	printf("%-20s%-30s\n", "BOLUM-ID", "BOLUM-ADI");
	while (fread(&b1, sizeof(bolum), 1, Ptr))
	{
		printf("%-20d%-30s\n", b1.bolumID, b1.bolumAd);
	}
	fclose(Ptr);
}

int bolumMenu()
{
	int secim;
	printf("\n\tBolum islemleri...\n\n");
	printf("\n\t1- Bolum Ekle \n");
	printf("\n\t2- Bolum listele \n");
	printf("\n\t0- Cikis \n");
	printf("\n\t1- Seciminiz :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void bolumIslemleri()
{
	int secim = bolumMenu();
	while (secim != 0)
	{
		switch (secim)
		{
		case 1:
			bolumEkle();
			break;
		case 2:
			bolumListele();
			break;
		case 0:
			break;
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = bolumMenu();
	}
	printf("bolum islemlerinden cikis yaptiniz ... \n");
}

void notEkle()
{
	system("cls");
	printf("Not ekleme islemi... \n\n");
	notlar n1;

	printf("Ogrenci Numarasi : ");
	scanf(" %d", &n1.ogrenciID);
	dersListele();
	printf("Ders Numarasi : ");
	scanf(" %d", &n1.dersID);

	bolumListele();
	printf("Bolum Numarasi : ");
	scanf(" %d", &n1.bolumID);

	ogretimGorevlisiListele();
	printf("Ogretim gorevlisi numarasi : ");
	scanf(" %d", &n1.ogrGorevlisiID);

	printf("Puani : ");
	scanf(" %f", &n1.puan);

	FILE *numPtr = fopen("./data/notNumaralari.dat", "a+b");
	int numara = 0;
	while (fread(&numara, sizeof(int), 1, numPtr))
	{
	}
	numara += 1;
	n1.notID = numara;
	fwrite(&numara, sizeof(int), 1, numPtr);
	fclose(numPtr);

	FILE *ptr = fopen("./data/notlar.dat", "a+b");
	fwrite(&n1, sizeof(notlar), 1, ptr);
	fclose(ptr);
	printf("%d numarali not kaydi tamam \n", numara);
}

void notListele()
{
	system("cls");
	printf("Not listesi ... \n\n");

	notlar n1;

	FILE *ptr = fopen("./data/notlar.dat", "r+b");
	printf("%-20s%-20s%-20s\n", "DERS-ID", "OGRENCI-NO", "PUAN");
	while (fread(&n1, sizeof(notlar), 1, ptr))
	{
		printf("%-20d%-20d%.2f\n", n1.dersID, n1.ogrenciID, n1.puan);
	}
	fclose(ptr);
}

int notMenu()
{
	int secim;
	printf("\n\tNot islemleri... \n\n");
	printf("\n\t1- Not Ekle  \n");
	printf("\n\t2- Not Listele \n");
	printf("\n\t0- Cikis \n");
	printf("\n\t1- Seciminiz   :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void notIslemleri()
{
	int secim = notMenu();
	while (secim != 0)
	{
		switch (secim)
		{
		case 1:
			notEkle();
			break;
		case 2:
			notListele();
			break;
		case 0:
			break;
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = notMenu();
	}
	printf("Not islemlerinden cikis yaptiniz ... \n");
}

int menu()
{
	int secim;
	// system("cls");
	printf("\n\tOGRENCI ISLERI OTOMASYONU\n\n");
	printf("\n\t1- OGRENCI ISLEMLERI \n");
	printf("\n\t2- OGRETIM GOREVLISI ISLEMLERI \n");
	printf("\n\t3- DERS ISLEMLERI \n");
	printf("\n\t4- BOLUM ISLEMLERI \n");
	printf("\n\t5- NOT ISLEMLERI \n");
	printf("\n\t9- KULLANICI ISLEMLERI \n");
	printf("\n\t0- PROGRAMI KAPAT \n");
	printf("\n\t1- Seciminiz   :  ");
	scanf("%d", &secim);
	system("cls");
	return secim;
}

void menuEkrani()
{
	int secim = menu();
	while (secim != 9)
	{
		switch (secim)
		{
		case 1:
			ogrenciIslemleri();
			break;
		case 2:
			ogretimGorevlisiIslemleri();
			break;
		case 3:
			dersIslemleri();
			break;
		case 4:
			bolumIslemleri();
			break;
		case 5:
			notIslemleri();
			break;
		case 9:
			break;
		case 0:
			printf("Program sonlandiriliyor... \n");
			exit(0);
		default:
			printf("Hatali secim yaptiniz ! \n");
		}
		secim = menu();
	}
}

int kullaniciAdiVarMi(Kullanici *kullanicilar, int kullaniciSayisi, const char *kullaniciAdi)
{
	int i;

	for (i = 0; i < kullaniciSayisi; i++)
	{
		if (strcmp(kullanicilar[i].kullaniciAdi, kullaniciAdi) == 0)
		{
			return 1; // Kullanici adi var
		}
	}
	return 0; // Kullanici adi yok
}

void kullaniciKayit(Kullanici *kullanicilar, int *kullaniciSayisi)
{
	if (*kullaniciSayisi >= MAX_KULLANICILAR)
	{
		printf("Maksimum kullanici sayisina ulasildi.\n");
		return;
	}

	Kullanici yeniKullanici;

	system("cls");
	printf("\n\tKULLANICI KAYIT ISLEMI ...\n\n\n");
	printf("\tKullanici Adi: ");
	scanf("%s", yeniKullanici.kullaniciAdi);

	if (kullaniciAdiVarMi(kullanicilar, *kullaniciSayisi, yeniKullanici.kullaniciAdi))
	{
		system("cls");
		printf("Bu kullanici adi zaten var. Baska bir kullanici adi seciniz.\n");
		return;
	}
	printf("\tSifre: ");
	scanf("%s", yeniKullanici.sifre);

	kullanicilar[*kullaniciSayisi] = yeniKullanici;
	(*kullaniciSayisi)++;

	// Kullanici bilgilerini binary dosyalara sona ekleme
	FILE *kullaniciDosyasi = fopen("./data/kullanici.dat", "ab");
	FILE *sifreDosyasi = fopen("./data/sifre.dat", "ab");

	system("cls");
	if (kullaniciDosyasi != NULL && sifreDosyasi != NULL)
	{
		fwrite(&yeniKullanici, sizeof(Kullanici), 1, kullaniciDosyasi);
		fwrite(&yeniKullanici, sizeof(Kullanici), 1, sifreDosyasi);

		fclose(kullaniciDosyasi);
		fclose(sifreDosyasi);

		printf("Kullanici kaydedildi.\n");
	}
	else
	{
		printf("Dosya acma hatasi.\n");
	}
}

int kullaniciGiris(Kullanici *kullanicilar, int kullaniciSayisi)
{
	char kullaniciAdi[KULLANICI_ADI_UZUNLUK];
	char sifre[SIFRE_UZUNLUK];
	int i;
	system("cls");
	printf("\n\tGIRIS YAPMA ISLEMI ...\n\n\n");

	printf("\tKullanici Adi: ");
	scanf("%s", kullaniciAdi);

	printf("\tSifre: ");
	scanf("%s", sifre);

	for (i = 0; i < kullaniciSayisi; i++)
	{
		if (strcmp(kullanicilar[i].kullaniciAdi, kullaniciAdi) == 0 && strcmp(kullanicilar[i].sifre, sifre) == 0)
		{
			return i; // Giris basarili
		}
	}

	return -1; // Giris basarisiz
}

void kullaniciBilgileriniOku(Kullanici *kullanicilar, int *kullaniciSayisi)
{
	FILE *kullaniciDosyasi = fopen("./data/kullanici.dat", "rb");
	FILE *sifreDosyasi = fopen("./data/sifre.dat", "rb");

	if (kullaniciDosyasi != NULL && sifreDosyasi != NULL)
	{
		while (fread(&kullanicilar[*kullaniciSayisi], sizeof(Kullanici), 1, kullaniciDosyasi) == 1)
		{
			fread(&kullanicilar[*kullaniciSayisi], sizeof(Kullanici), 1, sifreDosyasi);
			(*kullaniciSayisi)++;
		}

		fclose(kullaniciDosyasi);
		fclose(sifreDosyasi);
	}
}

int main()
{
	dataKlasoru();

	Kullanici kullanicilar[MAX_KULLANICILAR];
	int kullaniciSayisi = 0;
	int secim;

	kullaniciBilgileriniOku(kullanicilar, &kullaniciSayisi);

	do
	{
		printf("\n\t1. KAYIT OL\n");
		printf("\n\t2. GIRIS YAP\n");
		printf("\n\t0. CIKIS\n");
		printf("\n\tSeciminiz: ");
		scanf("%d", &secim);

		switch (secim)
		{
		case 1:
			kullaniciKayit(kullanicilar, &kullaniciSayisi);
			break;
		case 2:
		{
			int userindex = kullaniciGiris(kullanicilar, kullaniciSayisi);
			if (userindex != -1)
			{
				system("cls");
				printf("Giris basarili!\n");
				printf("\nHosgeldin, %s!", kullanicilar[userindex].kullaniciAdi);
				menuEkrani();
			}
			else
			{
				system("cls");
				printf("Giris basarisiz. Kullanici adi veya sifre hatali.\n");
			}
			break;
		}
		case 0:
			printf("Cikis yapiliyor...\n");
			break;
		default:
			system("cls");
			printf("Gecersiz secim. Lutfen tekrar deneyin.\n");
		}
	} while (secim != 0);

	printf("Programi kapattiniz... \n");

	return 0;
}
