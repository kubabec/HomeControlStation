/**
 * @file CyclicProfiler.hpp
 * @brief Diagnostic profiler used to measure periodic task execution time.
 */
#pragma once
#include <Arduino.h>
#include <map>

/**
 * @class CyclicProfiler
 * @brief Collects timing statistics for scheduled firmware tasks.
 *
 * The profiler wraps periodic OS callbacks and records execution duration,
 * backlog count, and maximum latency over time.
 */
class CyclicProfiler {
public:
  /** Aggregated statistics for one named task. */
  struct Stat {
    /** Total execution time accumulated across all invocations. */
    uint32_t totalTime = 0;
    /** Maximum single-run execution time observed for the task. */
    uint32_t maxTime   = 0;
    /** Number of times the task has been called. */
    uint32_t count     = 0;
    /** Number of missed iterations caused by delayed execution. */
    uint32_t backlog   = 0;
  };

  /** Signature used by profiled callbacks. */
  using FuncPtr = void(*)();

  /**
   * @brief Executes a task and records timing statistics under the provided key.
   * @param name Unique logical task name used in the profiling map.
   * @param func Pointer to the callback to execute and measure.
   */
  static void call(const char* name, FuncPtr func);

  /**
   * @brief Sets the number of iterations that the most recent profiled call performed.
   * @param it Number of loop iterations executed in the current tick window.
   */
  static void setLastIterations(int it) ;

  /**
   * @brief Schedules periodic reporting of accumulated task metrics.
   * @param ms Reporting interval in milliseconds.
   */
  static void reportEvery(unsigned long ms);

private:
  /** Mapping between task name and its accumulated statistics. */
  static inline std::map<String, Stat> stats;
  /** Time of the next scheduled reporting cycle. */
  static inline unsigned long nextReport      = 0;
  /** Amount of iterations that the current task wrapper should consider. */
  static inline int          currentIterations = 1;
};
