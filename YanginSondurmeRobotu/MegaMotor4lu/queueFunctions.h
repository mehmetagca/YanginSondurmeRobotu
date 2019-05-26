
#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>

template<class T>
class Queue {
  private:
    int _front, _back, _count;
    T *_data;
    int _maxitems;
  public:
    Queue(int maxitems = 256) { 
      _front = 0;
      _back = 0;
      _count = 0;
      _maxitems = maxitems;
      _data = new T[maxitems + 1];   
    }
    ~Queue() {
      delete[] _data;  
    }
    inline int count();
    inline int front();
    inline int back();
    void push(const T &item);
    T peek();
    T pop();
    void clear();
};

template<class T>
inline int Queue<T>::count() 
{
  return _count;
}

template<class T>
inline int Queue<T>::front() 
{
  return _front;
}

template<class T>
inline int Queue<T>::back() 
{
  return _back;
}

template<class T>
void Queue<T>::push(const T &item)
{
  if(_count < _maxitems) { // Drops out when full
    _data[_back++]=item;
    ++_count;
    // Check wrap around
    if (_back > _maxitems)
      _back -= (_maxitems + 1);
  }
}

template<class T>
T Queue<T>::pop() {
  if(_count <= 0) return T(); // Returns empty
  else {
    T result = _data[_front];
    _front++;
    --_count;
    // Check wrap around
    if (_front > _maxitems) 
      _front -= (_maxitems + 1);
    return result; 
  }
}

template<class T>
T Queue<T>::peek() {
  if(_count <= 0) return T(); // Returns empty
  else return _data[_front];
}

template<class T>
void Queue<T>::clear() 
{
  _front = _back;
  _count = 0;
}

#endif

/*
void setup()
{
  Serial.begin (9600);
  Queue<String> queue(10); // Max 10 chars in this queue
  queue.push("-11,");
  queue.push("-25,");
  queue.count(); // 2
  Serial.print(queue.pop()); 
  Serial.print(queue.pop());
  queue.push("-12,");
  queue.push("-19,");
  queue.count(); // 4
  Serial.print(queue.pop()); 
  Serial.print(queue.pop()); 
  queue.push("-23,");
  queue.push("-29,");
  queue.count(); 
  Serial.print(queue.pop()); 
  Serial.print(queue.pop()); 
  
  //struct Point { int x; int y; }
  //Queue<Point> points(5);
  //points.push(Point{2,4});
  //points.push(Point{5,0});
  //points.count(); // 2
  

}

void loop()
{
  
}
*/