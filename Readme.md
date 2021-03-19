# Optional

## Summary

本代码基本实现Java中的Optional功能，旨在C++11中提供简单处理空指针的方案。



## Design

`Optional<T>`应该用作为普通对象（即非指针使用），内部包装了一个指针`_ptr`（不支持列表，即`[]`类型）。

当调用`*`或`->`时，如果不为空，直接调用`_ptr`，否则抛出文件中定义的`NullptrException`错误。

其余方法尽可能与Java保持一致，细节如下。



## Usage

### Constructor

1. ```cpp
   Optional(T*);
   ```

   会检查T*是否为空，如果为空则抛出错误

2. ```cpp
   Optional(bool, T*);
   ```
   第一个参数为任意bool类型，次构造函数不会检查T*是否为空

3. ```cpp
   Optional<T>::empty();
   ```

   创建一个空对象（指针使用默认值初始化）

4. ```cpp
   Optional<T>::of(T* );	// 参数可为 T&
   ```

   如果传入指针，会检查是否为空，如果正常则包装起这个指针。如果传入引用，则创建新对象后再包装.

5. ```cpp
   Optional<T>::ofNullable(T* ); // 参数可为 T&
   ```

   如果传入指针，不会检查是否为空，直接包装起这个指针。如果传入引用，则创建新对象后再包装.



### Destructor

如果指针不为空，会触发`delete`函数释放空间



### Method

- ```cpp
  .get();
  ```

  获得包装内容的引用

- ```cpp
  .isPresent();
  ```

  返回一个`bool`，用于标记当前包装器内指针是否为空

- ```cpp
  .ifPresent(T_);
  ```

  要求传入一个参数类型为包装器内指针的类型 的可执行函数

  如果当前包装器内指针不为空，则执行该函数

- ```cpp
  .filter(T_);
  ```

  要求传入一个函数，其参数类型为包装器内指针的类型，返回值为`bool`

  如果当前包装器内指针不为空，并且函数的运算结果为true，则返回当前指针，否则返回空

- ```cpp
  .map(T_);
  ```

  要求传入一个函数，其参数类型为包装器内指针的类型，返回值**不为**`void`

  如果当前包装器内指针不为空，则把执行函数，并打包成新的`Optional`对象

- ```cpp
  .orElse(Optional& other);
  ```
  
  要求传入一个**同类型**的Optional
  
  如果当前包装器内指针为空，则返回`other`，否则返回包装器内指针
  
- ```
  .orElseGet(Optional& other)
  ```
  
  要求传入一个**同类型**的Optional

  如果当前包装器内指针为空，则返回`other.get()`，否则返回包装器内容的引用

- ```cpp
  .orElseThrow(T_& )
  ```

  要求传入一个用于表示异常状态的类，

  如果当前包装器内指针为空，则抛出该异常，否则返回包装器内指针

  

## Example

详见`example.cpp`

