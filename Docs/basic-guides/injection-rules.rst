Injection Rules
========================================

The dependency injection mechanism relies heavily on template metaprogramming and it has some limitations.

General
---------------------
* Only one constructor should be defined for each instance implementation
* If the service is registered with interface and implementation, the interface should have a virtual destructor
* If multiple services are registered by the same interface, all should have the same lifetime (the build method will throw an exception)
* Only one service implementation can be registered (the build method will throw an exception)

Injecting Services
---------------------
* Singleton/scoped services can be injected using one of:
    * References: (T&)
    * Const references: (const T&)
    * Pointers: (T*)
    * Const pointer: (T* const)
    * Pointer to const object: (const T*)
    * Const pointer to const object: (const T* const)
* Transient services can be injected using one of:
    * std::unique_ptr: (unique_ptr<T>)
    * In place object if type is movable or copyable: T
* Multiple services implementing specified interface can be injected using std::vector:
    * Transient (std::vector<std::unique_ptr<T>>)
    * Singleton/scoped (std::vector<T*>)

.. list-table:: Injection table
   :widths: 50 50
   :header-rows: 1

   * - Constructor param type
     - ServiceProvider method used
   * - T - if movable or copyable
     - provider.createServiceInPlace<T>()
   * - std::unique_ptr<T>
     - provider.createService<T>()
   * - T&
     - provider.getService<T>()
   * - T*
     - provider.tryGetService<T>()
   * - std::vector<T*>
     - provider.getServices<T>()
   * - std::vector<std::unique_ptr<T>>
     - provider.createServices<T>()

.. literalinclude:: ../../Examples/Guides/InjectionRules.cpp
   :caption: Examples/Guides/InjectionRules
   :language: C++
