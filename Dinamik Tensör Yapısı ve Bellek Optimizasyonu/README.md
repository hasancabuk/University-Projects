
# TinyML Dinamik Tensör Yapısı ve Bellek Optimizasyonu Projesi

Bu proje, yapay zeka modellerinin (Deep Learning, Neural Networks) mikrodenetleyiciler (ESP32, Arduino vb.) gibi RAM kısıtlı cihazlar üzerinde çalıştırılabilmesi için geliştirilmiş, C dilinde yazılmış düşük seviyeli (low-level) bir tensör kütüphanesidir.

## 🚀 Projenin Amacı ve Teknik Motivasyon
Geleneksel derin öğrenme modelleri ağırlıkları ve verileri **32-bit Floating Point (FP32)** formatında saklar. Ancak gömülü sistemlerde:
1. **Bellek Kısıtlıdır:** Birkaç yüz kilobaytlık RAM, büyük matrisleri saklamaya yetmez.
2. **Hız İhtiyacı:** Mikrodenetleyiciler tamsayı (integer) işlemlerini, ondalıklı işlemlere göre çok daha hızlı gerçekleştirir.

Bu proje, bu iki sorunu çözmek için **Dinamik Veri Yapıları** ve **Post-Training Quantization (Eğitim Sonrası Nicemleme)** tekniklerini kullanır.

---

## 🛠 Teknik Mimari ve Uygulama Detayları

### 1. Dinamik Tensör Yapısı (Union & Struct)
Projede, belleği manipüle etmek için C dilinin ilkel yapıları kullanılmıştır. Union yapısı sayesinde, aynı bellek alanı duruma göre farklı veri tiplerine ev sahipliği yapar. Bu durum, sistemin çalışma anında (runtime) esneklik kazanmasını sağlarken, gereksiz RAM kullanımını minimuma indirir.

### 2. Asimetrik Quantization (Nicemleme) Algoritması
Quantization, float değerleri [-128, 127] aralığına sıkıştırma işlemidir. Algoritmada kullanılan matematiksel model:

q = round(x / S) + Z

* **S (Scale):** Verinin dinamik aralığını 8-bitlik alana yayar.
* **Z (Zero-point):** Gerçek hayattaki 0.0 değerinin tam sayı karşılığıdır.
* **Clamping:** Hesaplanan değerlerin int8_t sınırlarını (-128, 127) aşmaması için sınırlama uygulanır.

---

## 📈 Performans Analizi
FP32'den INT8'e geçiş yapıldığında elde edilen kazanımlar:
- **Bellek Tasarrufu:** %75 (Örn: 400KB'lık bir katman 100KB'a düşer).
- **Enerji Verimliliği:** Daha az CPU döngüsü harcandığı için enerji tüketimi azalır.

---

## 💻 Kurulum ve Derleme (Build Instructions)
Proje, herhangi bir ek kütüphane gerektirmeksizin standart C derleyicileriyle derlenebilir.

1. Depoyu klonlayın:
   git clone https://github.com/kullaniciadi/TinyML-Tensor-Project.git

2. Kaynak kodun bulunduğu dizine gidin:
   cd TinyML-Tensor-Project

3. GCC ile derleyin:
   gcc main.c -o tensor_demo -lm

4. Uygulamayı çalıştırın:
   ./tensor_demo

---

#### **Geliştirici:** Hasan Çabuk
#### **Üniversite:** Kırklareli Üniversitesi 1.Sınıf