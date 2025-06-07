#include <os/CyclicProfiler.hpp>


  using FuncPtr = void(*)();

  // Wywołaj zamiast func(): zbieramy statystyki dla klucza `name`
void CyclicProfiler::call(const char* name, FuncPtr func) {
    unsigned long t0 = micros();
    func();
    unsigned long dt = micros() - t0;

    auto& s = stats[String(name)];
    s.totalTime += dt;
    s.count     += 1;
    s.backlog   += (currentIterations > 1) ? (currentIterations - 1) : 0;
    if (dt > s.maxTime) s.maxTime = dt;
  }

  // Zamiast iterować w każdej pętli, ustawiamy globalnie ile iteracji wykonało się
  // w ostatnim call() – nadpisywane przy każdym wywołaniu call()
void CyclicProfiler::setLastIterations(int it) {
    currentIterations = it;
  }

  // Wywołaj co `ms` milisekund, żeby zrobić raport i wyzerować statystyki
 void CyclicProfiler::reportEvery(unsigned long ms) {
    unsigned long now = millis();
    if (nextReport == 0) nextReport = now + ms;

    if (now >= nextReport) {
      Serial.println(F("------ Statystyki aplikacji cyclic() ------"));
      for (auto& kv : stats) {
        auto& name = kv.first;
        auto& s    = kv.second;
        if (s.count == 0) continue;

        float avg  = s.totalTime / (float)s.count;
        float load = s.totalTime / (float)(ms * 1000UL) * 100.0f;

        Serial.printf(" %s(): avg = %.1f µs, max = %lu µs, backlog = %lu, load = %.2f %%\n",
                      name.c_str(), (double)avg, s.maxTime, s.backlog, (double)load);

        // wyzeruj na kolejny okres
        s = Stat{};
      }
      Serial.println(F("--------------------------------------------"));

      nextReport += ms;
    }
  }

