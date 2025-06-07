#pragma once
#include <Arduino.h>
#include <map>

class CyclicProfiler {
public:
  struct Stat {
    uint32_t totalTime = 0;
    uint32_t maxTime   = 0;
    uint32_t count     = 0;
    uint32_t backlog   = 0;
  };

  using FuncPtr = void(*)();

  // Wywołaj zamiast func(): zbieramy statystyki dla klucza `name`
  static void call(const char* name, FuncPtr func);

  // Zamiast iterować w każdej pętli, ustawiamy globalnie ile iteracji wykonało się
  // w ostatnim call() – nadpisywane przy każdym wywołaniu call()
  static void setLastIterations(int it) ;

  // Wywołaj co `ms` milisekund, żeby zrobić raport i wyzerować statystyki
  static void reportEvery(unsigned long ms);

private:
  static inline std::map<String, Stat> stats;
  static inline unsigned long nextReport      = 0;
  static inline int          currentIterations = 1;
};
