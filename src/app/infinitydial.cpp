#include <infinitydial.h>

InfinityDial::InfinityDial(QWidget *parent) : QDial(parent) {
  setMinimum(0);
  setMaximum(100);
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleValueChanged(int)));
}

void InfinityDial::handleValueChanged(int value) {
  if (value > previousValue) {
    if (value - previousValue > 50) {
      emit valueIncreased(value - previousValue - 100);
    } else {
      emit valueIncreased(value - previousValue);
    }
  } else {
    if (previousValue - value > 50) {
      emit valueIncreased(value - previousValue + 100);
    } else {
      emit valueIncreased(value - previousValue);
    }
  }
  previousValue = value;
}

InfinityDial::~InfinityDial() = default;
