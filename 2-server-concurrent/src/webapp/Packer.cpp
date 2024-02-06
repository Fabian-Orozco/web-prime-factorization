// Copyright Espera Activa

#include <vector>

#include "NumbersStructure.hpp"
#include "Packer.hpp"


Packer::Packer() {
  this->createOwnQueue();
  this->producingQueue = new Queue<std::vector<Numbers>>();
}

Packer::~Packer() {
  assert(this->consumingQueue);
  if (this->consumingQueue) {
    delete(this->consumingQueue);
  }
  this->consumingQueue = nullptr;
}

Packer& Packer::getInstance() {
  static Packer packer;
  return packer;
}

int Packer::run() {
  this->consumeForever();
  return 1;
}

bool Packer::full(size_t position) {
  if (requests[position].size() == requests[position][0].numbersCount) {
    return true;
  }
  return false;
}

void Packer::consumeForever() {
  assert(this->consumingQueue);
  while (true) {
    // Get the next data to consume, or block while queue is empty
    const Numbers& currentNumber = this->consumingQueue->pop();
    // If data is the stop condition, stop the loop
    if ( currentNumber.numberIndex == -1 ) {
      break;
    }
    // Process this data
    this->consume(currentNumber);
  }
}

void Packer::consume(Numbers currentNumber) {
  size_t currentRequestNumber = currentNumber.requestNumber;
  try {
    this->requests.at(currentRequestNumber).push_back(currentNumber);
  } catch (const std::out_of_range& err) {
    this->requests.resize(currentRequestNumber + 1);
    std::vector<Numbers> add;
    this->requests[currentRequestNumber] = add;
    this->requests[currentRequestNumber].push_back(currentNumber);
  }

  if (full(currentRequestNumber)) {
    this->produce(requests[currentRequestNumber]);
    requests[currentRequestNumber].clear();
  }
}
