/*
 * %injeqt copyright begin%
 * Copyright 2016 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <injeqt/injector.h>
#include <injeqt/module.h>

#include <QtTest/QtTest>

class default_object;
class ready_object;
class factory_object;
class by_factory_object;

class common_object : public QObject
{
	Q_OBJECT

public:
	common_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	};

	virtual ~common_object()
	{
	}

protected:
	bool _common_init_called = false;
	bool _common_done_called = false;

private slots:
	INJEQT_INIT void init()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
		_common_init_called = true;
	}
	INJEQT_DONE void done()
	{
		QVERIFY(_common_init_called);
		QVERIFY(!_common_done_called);
		_common_done_called = true;
	}
};

class default_object : public common_object
{
	Q_OBJECT

public:
	Q_INVOKABLE default_object()
	{
		QVERIFY(!_init_called);
		QVERIFY(!_done_called);
	};

	virtual ~default_object()
	{
		QVERIFY(_init_called);
		QVERIFY(_done_called);
		QVERIFY(_common_init_called);
		QVERIFY(_common_done_called);
	}

private:
	QPointer<ready_object> _ready_object;
	QPointer<by_factory_object> _by_factory_object;
	bool _init_called = false;
	bool _done_called = false;

private slots:
	INJEQT_INIT void init()
	{
		QVERIFY(_ready_object);
		QVERIFY(_by_factory_object);
		QVERIFY(_common_init_called);
		QVERIFY(!_common_done_called);
		QVERIFY(!_init_called);
		QVERIFY(!_done_called);
		_init_called = true;
	}
	INJEQT_DONE void done()
	{
		QVERIFY(_ready_object);
		QVERIFY(_by_factory_object);
		QVERIFY(_common_init_called);
		QVERIFY(!_common_done_called);
		QVERIFY(_init_called);
		QVERIFY(!_done_called);
		_done_called = true;
	}
	INJEQT_SET void set_ready_object(ready_object *x)
	{
		QVERIFY(!_init_called);
		QVERIFY(!_done_called);
		_ready_object = x;
	}
	INJEQT_SET void set_by_factory_object(by_factory_object *x)
	{
		QVERIFY(!_init_called);
		QVERIFY(!_done_called);
		_by_factory_object = x;
	}

};

class by_factory_object : public common_object
{
	Q_OBJECT

public:
	Q_INVOKABLE by_factory_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	};

	virtual ~by_factory_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	}

private slots:
	INJEQT_INIT void init()
	{
		QVERIFY(false);
	}
	INJEQT_DONE void done()
	{
		QVERIFY(false);
	}

};

class factory : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE factory() {}
	Q_INVOKABLE by_factory_object * create_by_factory_object() const { return new by_factory_object{}; }

};

class ready_object : public common_object
{
	Q_OBJECT

public:
	Q_INVOKABLE ready_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	};

	virtual ~ready_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	}

private slots:
	INJEQT_INIT void init()
	{
		QVERIFY(false);
	}
	INJEQT_DONE void done()
	{
		QVERIFY(false);
	}

};

class injeqt_into_object : public common_object
{
	Q_OBJECT

public:
	Q_INVOKABLE injeqt_into_object()
	{
		QVERIFY(!_common_init_called);
		QVERIFY(!_common_done_called);
	};

	virtual ~injeqt_into_object()
	{
		QVERIFY(_init_called);
		QVERIFY(_common_init_called);
		QVERIFY(!_common_done_called);
	}

private:
	QPointer<default_object> _default_object;
	QPointer<ready_object> _ready_object;
	QPointer<by_factory_object> _by_factory_object;
	bool _init_called = false;

private slots:
	INJEQT_INIT void init()
	{
		QVERIFY(_default_object);
		QVERIFY(_ready_object);
		QVERIFY(_by_factory_object);
		QVERIFY(_common_init_called);
		QVERIFY(!_common_done_called);
		_init_called = true;
	}
	INJEQT_DONE void done()
	{
		QVERIFY(false);
	}
	INJEQT_SET void set_default_object(default_object *x)
	{
		QVERIFY(!_init_called);
		QVERIFY(!_common_init_called);
		_default_object = x;
	}
	INJEQT_SET void set_ready_object(ready_object *x)
	{
		QVERIFY(!_init_called);
		QVERIFY(!_common_init_called);
		_ready_object = x;
	}
	INJEQT_SET void set_by_factory_object(by_factory_object *x)
	{
		QVERIFY(!_init_called);
		QVERIFY(!_common_init_called);
		_by_factory_object = x;
	}

};

class init_done_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_call_init_and_done();

};

void init_done_test::should_properly_call_init_and_done()
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			_ready_object = std::unique_ptr<ready_object>(new ready_object{});
			add_type<default_object>();
			add_type<factory>();
			add_factory<by_factory_object, factory>();
			add_ready_object<ready_object>(_ready_object.get());
		}
		virtual ~m() {}
	private:
		std::unique_ptr<ready_object> _ready_object;
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	auto injector = injeqt::injector{std::move(modules)};

	injector.get<default_object>();
	injector.get<by_factory_object>();
	injector.get<ready_object>();

	injeqt_into_object object{};
	injector.inject_into(&object);
}

QTEST_APPLESS_MAIN(init_done_test)
#include "init-done-test.moc"
