injeqt
======

Dependency injection framework for Qt

Description
-----------

Dependency injection is an implementation of inverse of control and can be used
to increase program modularity and extendability. In general it also results in more
testable source code.

There is a lot of dependency injection frameworks for bytecode based languages like
Java and C#, for instance Guice and Spring. C++ lacks reflection facilities that make
dependency injection so powerfull in these languages.

Injeqt is an attempt to build a powerful and reliable dependency injection framework
on Qt's reflection based on MOC (meta-object compiler). For version 0.1 some basic
features were implemented.

Features
--------

*QObject based services*

All services dependencies in injeqt must be QObject descendents. This is requirement
of Qt meta object system.

*Setter-based injection of QObject services*

All dependencies are injected by setters. Setters are defined in `private slots` part
of class and must be marked with `INJEQT_SET` tag (`INJEQT_SETTER` is also available for
backwards compatibility).

Additional slots can be marked with `INJEQT_INIT` and `INJEQT_DONE` tags. `INJEQT_INIT`
methods are called after all injeqt setters are called - this allows for initializing of
object when all services are available. `INJEQT_DONE` methods are called before injector
with all its object is destroyed. This allows for gracefull shutdown with all services
still available (destruction will follow calling all `INJEQT_DONE` methods in system).

*Several methods of object creation*

Injeqt can create object using default contructor or factories. It can also receive ready
objects from outside (called ready-objects). Resolving of dependencies and calling injeqt
setters, init and done methods is only done for objects created by injeqt itself - with
default constructor method (added with add_type<T>()). It can change in future versions
of injeqt.

*Injection into objects*

Injeqt can also call `INJEQT_SET` methods on objects created outside injector infrastructure.
Call injector.injeqt_into(QObject *) to do that. It will also call `INJEQT_INIT` methods
after setting all services. No `INJEQT_DONE` methods are called.

*Small interface*

Injeqt exposes only very limited interface to allow internals to change freely while maintaining
ABI and API.

Qt relationshipt
----------------

Injeqt requires Qt for its reflection capabilities.

*QObject*

Injeqt only recognizes QObject-derived types. That may change when reflection get accepted into one
of the next C++ standards.

Plans
-----

*1.1*

* add on-demand and immediate object creation modes

*1.2*

* more initialization checking (searching factory loops)

*1.3*

* (maybe) contructor injection
* (maybe) more dynamic type and dependencies resolutions

Example
-------

Here is example of what can be done using injeqt:

	#include <injeqt/injector.h>
	#include <injeqt/module.h>

	#include <QtCore/QObject>
	#include <iostream>
	#include <memory>
	#include <string>

	class hello_service : public QObject
	{
		Q_OBJECT

	public:
		hello_service() {}
		virtual ~hello_service() {}

		std::string say_hello() const
		{
			return {"Hello"};
		}
	};

	class world_service : public QObject
	{
		Q_OBJECT

	public:
		world_service() {}
		virtual ~world_service() {}

		std::string say_world() const
		{
			return {"World"};
		}
	};

	class hello_factory : public QObject
	{
		Q_OBJECT

	public:
		Q_INVOKABLE hello_factory() {}
		virtual ~hello_factory() {}

		Q_INVOKABLE hello_service * create_service()
		{
			return new hello_service{};
		}
	};

	class hello_client : public QObject
	{
		Q_OBJECT

	public:
		Q_INVOKABLE hello_client() : _s{nullptr}, _w{nullptr} {}
		virtual ~hello_client() {}

		std::string say() const
		{
			return _s->say_hello() + " " + _w->say_world() + "!";
		}

	private slots:
		INJEQT_INIT void init()
		{
			std::cerr << "all services set" << std::endl;
		}

		INJEQT_DONE void done()
		{
			std::cerr << "ready for destruction" << std::endl;
		}

		INJEQT_SET void set_hello_service(hello_service *s)
		{
			_s = s;
		}

		INJEQT_SET void set_world_service(world_service *w)
		{
			_w = w;
		}

	private:
		hello_service *_s;
		world_service *_w;

	};

	class module : public injeqt::module
	{
	public:
		explicit module()
		{
			_w = std::unique_ptr<world_service>{new world_service{}};

			add_type<hello_client>();
			add_type<hello_factory>();
			add_factory<hello_service, hello_factory>();
			add_ready_object<world_service>(_w.get());
		}

		virtual ~module() {}

	private:
		std::unique_ptr<world_service> _w;

	};

	int main()
	{
		auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
		modules.emplace_back(std::unique_ptr<injeqt::module>{new module{}});

		auto injector = injeqt::injector{std::move(modules)};
		auto client = injector.get<hello_client>();
		auto hello = client->say();

		std::cout << hello << std::endl;
	}

	#include "hello-world.moc"

In that example we can see two main services names `hello_service` and `world_service`. There
is also client of these names `hello_client`. In `module` class we configure how we create
and access these instances.

`hello_client` is added using `add_type` function. It means that injeqt will try to create it
using default constructor. We provide that by declaration of `Q_INVOKABLE hello_client()`
(`Q_INVOKABLE` is requires by Qt's meta object system).

`hello_service` is added using `add_factory` function. It means that injeqt will first try to
create a `hello_factory` object, then it will look for a method of that objet that returns
`hello_service` object. It will find `Q_INVOKABLE hello_service * create_service()` and use it.

To be able to create `hello_factory` injeqt must know about it, so we also add it using `add_type`
method.

Last, `world_service`, is added as a ready object - provided from outside of injeqt scope.

In `main` method list of conifguration modules are passwed to newly created `injector` instance.
From that moment, we can use `injector` to create and manage our services. Just one line below
an `hello_client` instance is required. This is what happens next:

* injeqt looks for dependencies of `hello_client` and found that it first needs to create `hello_factory`
* injeqt creates `hello_factory` without problems, as it does not have dependencies of its own
* injeqt adds new instance to object pool
* injeqt calls `hello_factory::create_service()` methods and ads its result to object pool
* now all dependencies of `hello_client` are available, so new instance of it is created with
  default constructor and its added to objec tpool
* all methods of `hello_client` marked with `INJEQT_SET` are called with proper objects from pool
* all methods of `hello_client` marked with `INJEQT_INIT` are called
* this instance is returned to caller
* before injector is destructed, all methods of `hello_client` marked with `INJEQT_DONE` are called
