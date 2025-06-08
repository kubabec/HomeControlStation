#include <Arduino.h>
#include <os/HomeStation_os.hpp>
#include <os/CyclicProfiler.hpp>

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  OperatingSystem::init();

}

void loop() {
  static unsigned long next2ms   = 0;
  static unsigned long next10ms   = 0;
  static unsigned long next20ms   = 0;
  static unsigned long next50ms   = 0;
  static unsigned long next1s     = 0;
  static unsigned long nextReport = 0;
  const   unsigned long REPORT_PERIOD_MS = 15000;
  const   unsigned long REPORT_PERIOD_US = REPORT_PERIOD_MS * 1000UL;

  // Statystyki czasowe (w µs) i backlogi
  static uint32_t sum2=0, cnt2=0, max2=0, backlog2=0;
  static uint32_t sum10=0, cnt10=0, max10=0, backlog10=0;
  static uint32_t sum20=0, cnt20=0, max20=0, backlog20=0;
  static uint32_t sum50=0, cnt50=0, max50=0, backlog50=0;
  static uint32_t sum1s=0, cnt1s=0, max1s=0, backlog1s=0;

  unsigned long now   = millis();
  unsigned long nowUs = micros();

  // Pierwsza inicjalizacja
  if (nextReport == 0) {
    nextReport = now + REPORT_PERIOD_MS;
    next2ms = now;
    next10ms   = now;
    next20ms   = now;
    next50ms   = now;
    next1s     = now;
  }

  // --- task2ms ---

  {
    int iterations = 0;
    while (now >= next2ms) {
      iterations++;
      unsigned long t0 = micros();
      OperatingSystem::task2ms();
      unsigned long dt = micros() - t0;
      sum2 += dt; 
      cnt2++;
      if (dt > max2) max2 = dt;
      next2ms += 2;
    }
    if (iterations > 1) backlog2 += (iterations - 1);
  }

  // --- task10ms ---
  {
    int iterations = 0;
    while (now >= next10ms) {
      iterations++;
      unsigned long t0 = micros();
      OperatingSystem::task10ms();
      unsigned long dt = micros() - t0;
      sum10 += dt; 
      cnt10++;
      if (dt > max10) max10 = dt;
      next10ms += 10;
    }
    if (iterations > 1) backlog10 += (iterations - 1);
  }

  // --- task20ms ---
  {
    int iterations = 0;
    while (now >= next20ms) {
      iterations++;
      unsigned long t0 = micros();
      OperatingSystem::task20ms();
      unsigned long dt = micros() - t0;
      sum20 += dt; 
      cnt20++;
      if (dt > max20) max20 = dt;
      next20ms += 20;
    }
    if (iterations > 1) backlog20 += (iterations - 1);
  }

  // --- task50ms ---
  {
    int iterations = 0;
    while (now >= next50ms) {
      iterations++;
      unsigned long t0 = micros();
      OperatingSystem::task50ms();
      unsigned long dt = micros() - t0;
      sum50 += dt; 
      cnt50++;
      if (dt > max50) max50 = dt;
      next50ms += 50;
    }
    if (iterations > 1) backlog50 += (iterations - 1);
  }

  // --- task1s ---
  {
    int iterations = 0;
    while (now >= next1s) {
      iterations++;
      unsigned long t0 = micros();
      OperatingSystem::task1s();
      unsigned long dt = micros() - t0;
      sum1s += dt; 
      cnt1s++;
      if (dt > max1s) max1s = dt;
      next1s += 1000;
    }
    if (iterations > 1) backlog1s += (iterations - 1);
  }

  // --- raport co 15 sekund ---
  if (now >= nextReport) {
    Serial.println(F("=== Statystyki czasowe OS (15s) ==="));

    if (cnt10) {
      float load10 = sum10 / (float)REPORT_PERIOD_US * 100.0f;
      Serial.printf(" task10ms(): avg = %.1f µs, max = %lu µs, backlog = %lu, load = %.2f %%\n",
                    sum10/(float)cnt10, max10, backlog10, load10);
    }
    if (cnt20) {
      float load20 = sum20 / (float)REPORT_PERIOD_US * 100.0f;
      Serial.printf(" task20ms(): avg = %.1f µs, max = %lu µs, backlog = %lu, load = %.2f %%\n",
                    sum20/(float)cnt20, max20, backlog20, load20);
    }
    if (cnt50) {
      float load50 = sum50 / (float)REPORT_PERIOD_US * 100.0f;
      Serial.printf(" task50ms(): avg = %.1f µs, max = %lu µs, backlog = %lu, load = %.2f %%\n",
                    sum50/(float)cnt50, max50, backlog50, load50);
    }
    if (cnt1s) {
      float load1s = sum1s / (float)REPORT_PERIOD_US * 100.0f;
      Serial.printf(" task1s():   avg = %.1f µs, max = %lu µs, backlog = %lu, load = %.2f %%\n",
                    sum1s/(float)cnt1s, max1s, backlog1s, load1s);
    }

    Serial.println(F("==================================="));

    // Zerowanie statystyk i umówienie kolejnego raportu
    sum10 = cnt10 = max10 = backlog10 = 0;
    sum20 = cnt20 = max20 = backlog20 = 0;
    sum50 = cnt50 = max50 = backlog50 = 0;
    sum1s = cnt1s = max1s = backlog1s = 0;
    nextReport += REPORT_PERIOD_MS;
  }

  CyclicProfiler::reportEvery(15000);
}
