#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // int8_t kullanımı için standart kütüphane

// 1. ENUM ve TYPEDEF: Hangi veri tipini kullandığımızı takip etmek için
typedef enum {
    TIP_FLOAT32,
    TIP_INT8
} VeriTipi;

// 2. UNION: Tensörümüz ya float dizisi ya da int8 dizisi olacak.
// İki pointer aynı bellek adresini paylaşır, hangisine yer ayırırsak onu kullanırız.
typedef union {
    float* f32_dizi;
    int8_t* i8_dizi;
} TensorVerisi;

// 3. STRUCT: Ana Tensör yapımız
typedef struct {
    int satir;
    int sutun;
    VeriTipi tip;
    TensorVerisi veri; // Union'u struct içine gömüyoruz
} Tensor;

// Dinamik Tensör Oluşturma Fonksiyonu (Pointer ve Malloc burada devreye giriyor)
Tensor* tensor_olustur(int satir, int sutun, VeriTipi tip) {
    // Önce Tensörün kendi iskeleti için bellek ayırıyoruz
    Tensor* yeni_tensor = (Tensor*)malloc(sizeof(Tensor));
    yeni_tensor->satir = satir;
    yeni_tensor->sutun = sutun;
    yeni_tensor->tip = tip;

    int toplam_eleman = satir * sutun;

    // Seçilen tipe göre sadece ilgili dizi için bellek tahsis ediyoruz
    if (tip == TIP_FLOAT32) {
        yeni_tensor->veri.f32_dizi = (float*)malloc(toplam_eleman * sizeof(float));
    } else if (tip == TIP_INT8) {
        yeni_tensor->veri.i8_dizi = (int8_t*)malloc(toplam_eleman * sizeof(int8_t));
    }

    return yeni_tensor;
}

int main() {
    int satir = 2, sutun = 3;
    int toplam_eleman = satir * sutun;

    // --- ADIM 1: Orijinal Yapay Zeka Modelini (32-bit Float) Oluştur ---
    Tensor* model_agirliklari = tensor_olustur(satir, sutun, TIP_FLOAT32);

    // Matrise örnek ağırlık değerleri atayalım
    model_agirliklari->veri.f32_dizi[0] = 1.25f;
    model_agirliklari->veri.f32_dizi[1] = -3.80f;
    model_agirliklari->veri.f32_dizi[2] = 5.10f; // Sıkıştırma için bu en büyük değeri (max) baz alacağız
    model_agirliklari->veri.f32_dizi[3] = 4.50f;
    model_agirliklari->veri.f32_dizi[4] = -0.45f;
    model_agirliklari->veri.f32_dizi[5] = 2.00f;

    printf("--- Orjinal Model Agirliklari (32-bit Float) ---\n");
    for (int i = 0; i < toplam_eleman; i++) {
        printf("%.2f  ", model_agirliklari->veri.f32_dizi[i]);
    }
    printf("\nBellek Tuketimi: %ld Byte\n\n", toplam_eleman * sizeof(float));

    // --- ADIM 2: Quantization (Bellek Kısıtlı Cihazlar İçin Sıkıştırma) ---
    Tensor* quantize_edilmis_model = tensor_olustur(satir, sutun, TIP_INT8);
    float max_deger = 5.10f; 

    for (int i = 0; i < toplam_eleman; i++) {
        // Değeri -127 ile +127 arasına oranla
        float oranlanmis_deger = (model_agirliklari->veri.f32_dizi[i] / max_deger) * 127.0f;
        
        // TİP DÖNÜŞÜMÜ (Type Casting): Float değeri 8-bit tam sayıya zorluyoruz
        quantize_edilmis_model->veri.i8_dizi[i] = (int8_t)oranlanmis_deger;
    }

    printf("--- Quantize Edilmis Agirliklar (8-bit Int) ---\n");
    for (int i = 0; i < toplam_eleman; i++) {
        printf("%d  ", quantize_edilmis_model->veri.i8_dizi[i]);
    }
    printf("\nBellek Tuketimi: %ld Byte\n\n", toplam_eleman * sizeof(int8_t));

    // --- ADIM 3: Bellek Temizliği (Memory Leak Önleme) ---
    // Önce içerdeki dizileri, sonra tensörün iskeletini serbest bırakıyoruz
    free(model_agirliklari->veri.f32_dizi);
    free(model_agirliklari);
    
    free(quantize_edilmis_model->veri.i8_dizi);
    free(quantize_edilmis_model);

    return 0;
}