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

class int_container : public QObject
{
	Q_OBJECT

public:
	int_container(int value) : _value{value} {}
	int value() const { return _value; }

private:
	int _value;

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

class module_1 : public injeqt::module
{
public:
	module_1(int value)
	{
		_container = std::unique_ptr<int_container>(new int_container{value});

		add_type<int_service>();
		add_ready_object<int_container>(_container.get());
	}

	virtual ~module_1() {}

private:
	std::unique_ptr<int_container> _container;
};

class ready_object_behavior_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_proper_object_structure()
	{
		auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
		modules.emplace_back(std::unique_ptr<module_1>{new module_1{9}});
		auto injector = injeqt::injector{std::move(modules)};
		auto service = injector.get<int_service>();
		QCOMPARE(9, service->value());
	}

};

QTEST_APPLESS_MAIN(ready_object_behavior_test)
#include "ready-object-behavior-test.moc"
