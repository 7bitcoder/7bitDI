Injection Rules
========================================

The dependency injection mechanism relies heavily on template metaprogramming and it has some limitations. The most important one is that we cannot inject service into service using reference.

General
---------------------
* Only one constructor should be defined for each service implementation
* If the service is registered with interface and implementation, the interface should have a virtual destructor
* If multiple services are registered by the same interface, all should have the same lifetime (the build method will throw an exception)
* Only one service implementation can be registered (the build method will throw an exception)
* It is guaranteed that injected service won't be null

Injecting Services
---------------------
* Services cannot be injected by value: (T)
* Singleton/scoped services can be injected using one of:

  * Pointers: (T*)
  * Const pointner: (T* const)
  * Pointner to const object: (const T*)
  * Const pointer to const object: (const T* const)
* Transient services can be injected using std::unique_ptr: (unique_ptr<T>)
* Multiple services implementing specified interface can be injected using std::vector:

  * Transient (std::vector<std::unique_ptr<T>>)
  * Singleton/scoped (std::vector<T*>)

.. literalinclude:: ../../Examples/Guides/InjectionRules.cpp
   :caption: Examples/Guides/InjectionRules
   :language: C++


