injeqt
======

Simple dependency injection framework for Qt

Documentation
-------------

Documentation available at [github](https://github.com/vogel/injeqt/wiki).

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

In `main` method list of conifguration modules are passed to newly created `injector` instance.
From that moment, we can use `injector` to create and manage our services. Just one line below
an `hello_client` instance is required. This is what happens next:

* injeqt looks for dependencies of `hello_client` and found that it first needs to create `hello_factory`
* injeqt creates `hello_factory` without problems, as it does not have dependencies of its own
* injeqt adds new instance to object pool
* injeqt calls `hello_factory::create_service()` methods and adds its result to object pool
* now all dependencies of `hello_client` are available, so new instance of it is created with
  default constructor and its added to objec tpool
* all methods of `hello_client` marked with `INJEQT_SET` are called with proper objects from pool
* all methods of `hello_client` marked with `INJEQT_INIT` are called
* this instance is returned to caller
* before injector is destructed, all methods of `hello_client` marked with `INJEQT_DONE` are called
