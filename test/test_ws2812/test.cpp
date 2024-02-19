#include <Arduino.h>
#include "PSEBoard.h"
#include "unity.h"

void setUp(void) {
    // Procedure di setup
}

void tearDown(void) {
    // Procedure di pulizia
}

void test_function(void) {
  TEST_ASSERT_TRUE_MESSAGE(1 == 1, "Fail message");
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_function);
  UNITY_END();
}

void loop() {
    // Zona in loop, poco utilizzata in test su piattaforma Embedded
}