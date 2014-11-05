/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

class default_constructible : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE default_constructible() {}

};

class int_container : public QObject
{
	Q_OBJECT

public:
	int_container() {}
	virtual ~int_container() {}
	virtual int value() const = 0;

};

class simple_int_container : public int_container
{
	Q_OBJECT

public:
	simple_int_container(int value) : _value{value} {}
	virtual ~simple_int_container() {}
	virtual int value() const override { return _value; }

private:
	int _value;

};

class int_container_with_ignored_setter : public int_container
{
	Q_OBJECT

public:
	int_container_with_ignored_setter(int value) : _value{value} {}
	virtual ~int_container_with_ignored_setter() {}
	virtual int value() const override { return _value; }
	default_constructible * object() const { return _object; }

private slots:
	INJEQT_SETTER void set_default_constructible(default_constructible *object)
	{
		_object = object;
	}

private:
	int _value;
	QPointer<default_constructible> _object;

};

class int_service : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE int_service() {}
	int value() const { return _int_container->value(); }

private slots:
	INJEQT_SETTER void set_int_container(int_container *container)
	{
		_int_container = container;
	}

private:
	QPointer<int_container> _int_container;

};

class ready_object_behavior_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_proper_object_structure();
	void should_ignore_invalid_setters_in_ready_object();
	void should_ignore_valid_setters_in_ready_object();

};

void ready_object_behavior_test::should_create_proper_object_structure()
{
	class m : public injeqt::module
	{
	public:
		m(int value)
		{
			_container = std::unique_ptr<simple_int_container>(new simple_int_container{value});
			add_type<int_service>();
			add_ready_object<simple_int_container>(_container.get());
		}
		virtual ~m() {}
	private:
		std::unique_ptr<simple_int_container> _container;
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{9}});
	auto injector = injeqt::injector{std::move(modules)};
	auto service = injector.get<int_service>();
	QCOMPARE(9, service->value());
}

void ready_object_behavior_test::should_ignore_invalid_setters_in_ready_object()
{
	class m : public injeqt::module
	{
	public:
		m(int value)
		{
			_container = std::unique_ptr<int_container_with_ignored_setter>(new int_container_with_ignored_setter{value});
			add_type<int_service>();
			add_ready_object<int_container_with_ignored_setter>(_container.get());
		}
		virtual ~m() {}
	private:
		std::unique_ptr<int_container_with_ignored_setter> _container;
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{9}});
	auto injector = injeqt::injector{std::move(modules)};
	auto service = injector.get<int_service>();
	auto container = injector.get<int_container_with_ignored_setter>();
	QCOMPARE(9, service->value());
	QCOMPARE(static_cast<QObject *>(nullptr), container->object());
}

void ready_object_behavior_test::should_ignore_valid_setters_in_ready_object()
{
	class m : public injeqt::module
	{
	public:
		m(int value)
		{
			_container = std::unique_ptr<int_container_with_ignored_setter>(new int_container_with_ignored_setter{value});
			add_type<int_service>();
			add_type<default_constructible>();
			add_ready_object<int_container_with_ignored_setter>(_container.get());
		}
		virtual ~m() {}
	private:
		std::unique_ptr<int_container_with_ignored_setter> _container;
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{9}});
	auto injector = injeqt::injector{std::move(modules)};
	auto service = injector.get<int_service>();
	auto container = injector.get<int_container_with_ignored_setter>();
	QCOMPARE(9, service->value());
	QCOMPARE(static_cast<QObject *>(nullptr), container->object());
}

QTEST_APPLESS_MAIN(ready_object_behavior_test)
#include "ready-object-behavior-test.moc"
