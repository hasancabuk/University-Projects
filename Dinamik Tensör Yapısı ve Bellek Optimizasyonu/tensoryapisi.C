#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Veri tiplerini tanımlayalım
typedef enum {
    T_FLOAT32,
    T_FLOAT16,
    T_INT8
} TensorType;

// Union: Aynı bellek alanını farklı tipler için kullanır (Bellek tasarrufu sağlar)
typedef union {
    float* f32;
    uint16_t* f16; // FP16 standardı C'de ilkel tip değildir, uint16 olarak tutulur
    int8_t* i8;
} TensorData;

typedef struct {
    uint16_t rows;
    uint16_t cols;
    TensorType type;
    TensorData data;
} Tensor;

// Tensör oluşturma fonksiyonu
Tensor* create_tensor(uint16_t r, uint16_t c, TensorType type) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->rows = r;
    t->cols = c;
    t->type = type;
    size_t size = r * c;

    if (type == T_FLOAT32) t->data.f32 = (float*)malloc(size * sizeof(float));
    else if (type == T_INT8) t->data.i8 = (int8_t*)malloc(size * sizeof(int8_t));
    else if (type == T_FLOAT16) t->data.f16 = (uint16_t*)malloc(size * sizeof(uint16_t));
    
    return t;
}

void quantize_tensor(Tensor* input, Tensor* output) {
    if (input->type != T_FLOAT32 || output->type != T_INT8) return;

    int n = input->rows * input->cols;
    float max_val = input->data.f32[0];
    float min_val = input->data.f32[0];

    // 1. Min ve Max değerleri bul (Aralık belirleme)
    for (int i = 1; i < n; i++) {
        if (input->data.f32[i] > max_val) max_val = input->data.f32[i];
        if (input->data.f32[i] < min_val) min_val = input->data.f32[i];
    }

    // 2. Scale ve Zero-point hesapla
    float scale = (max_val - min_val) / 255.0f;
    int8_t zero_point = (int8_t)(round(-min_val / scale) - 128);

    // 3. Dönüştürme (Quantize işlemi)
    for (int i = 0; i < n; i++) {
        float q = round(input->data.f32[i] / scale) + zero_point;
        // Clamping: -128 ile 127 arasına sıkıştır
        if (q > 127) q = 127;
        if (q < -128) q = -128;
        output->data.i8[i] = (int8_t)q;
    }
    printf("Quantization tamamlandi. Scale: %f, Zero-point: %d\n", scale, zero_point);
}


int main() {
    // 2x2'lik bir Float tensör oluştur
    Tensor* f_tensor = create_tensor(2, 2, T_FLOAT32);
    f_tensor->data.f32[0] = 0.5f;  f_tensor->data.f32[1] = -1.2f;
    f_tensor->data.f32[2] = 3.14f; f_tensor->data.f32[3] = 0.0f;

    // Quantize edilmiş halini tutacak tensör
    Tensor* q_tensor = create_tensor(2, 2, T_INT8);

    quantize_tensor(f_tensor, q_tensor);

    // Sonuçları yazdır
    printf("Quantized Degerler: %d, %d, %d, %d\n", 
            q_tensor->data.i8[0], q_tensor->data.i8[1], 
            q_tensor->data.i8[2], q_tensor->data.i8[3]);

    // Belleği serbest bırak (Gömülü sistemlerde hayati önem taşır!)
    free(f_tensor->data.f32); free(f_tensor);
    free(q_tensor->data.i8);  free(q_tensor);

    return 0;
}