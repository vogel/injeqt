/*
 * %injeqt copyright begin%
 * Copyright 2015 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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
	int_container() {}
	virtual ~int_container() {}
	virtual int value() const = 0;

};

class nine_container : public int_container
{
	Q_OBJECT

public:
	Q_INVOKABLE nine_container() {}
	virtual ~nine_container() {}
	virtual int value() const override { return 9; }

};

class int_service : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE int_service() {}
	int value() const { return _int_container->value(); }

private slots:
	INJEQT_SET void set_int_container(int_container *container)
	{
		_int_container = container;
	}

private:
	QPointer<int_container> _int_container;

};

class int_sub_service : public int_service
{
	Q_OBJECT

public:
	Q_INVOKABLE int_sub_service() {}

};

class inject_into_behavior_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_inject_into();

};

void inject_into_behavior_test::should_properly_inject_into()
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<nine_container>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	auto injector = injeqt::injector{std::move(modules)};

	int_service service{};
	injector.inject_into(&service);
	QCOMPARE(9, service.value());

	int_sub_service sub_service{};
	injector.inject_into(&sub_service);
	QCOMPARE(9, sub_service.value());
}

QTEST_APPLESS_MAIN(inject_into_behavior_test)
#include "inject-into-behavior-test.moc"
