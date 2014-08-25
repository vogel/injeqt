injeqt
======

Dependency injection framework for Qt

Dependency injection is an implementation of inverse of control and can be used
to increase program modularity and extendability. In general it also results in more
testable source code.

There is a lot of dependency injection frameworks for bytecode based languages like
Java and C#, for instance Guice and Spring. C++ lacks reflection facilities that make
dependency injection so powerfull in these languages.

Injeqt is an attept to build a powerfull and reliable dependency injection framework
on Qt's reflection based on MOC (meta-object compiler). For version 0.1 some basic
features were implemented:

* setter-based injection of QObject services
* several method of object creation
* support for factories

Here is example of what can be done using injeqt:

	#include "injector.h"
	#include "injeqt.h"
	#include "module.h"

	#include <QtCore/QDebug>
	#include <QtCore/QObject>
	#include <memory>

	class hello_service : public QObject
	{
		Q_OBJECT

	public:
		hello_service() {}
		virtual ~hello_service() {}

		QString say_hello() const
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

		QString say_world() const
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

		QString say() const
		{
			return _s->say_hello() + " " + _w->say_world() + "!";
		}

	private slots:
		INJEQT_SETTER void set_hello_service(hello_service *s)
		{
			_s = s;
		}

		INJEQT_SETTER void set_world_service(world_service *w)
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

		qDebug() << hello;
	}

	#include "hello-world.moc"
