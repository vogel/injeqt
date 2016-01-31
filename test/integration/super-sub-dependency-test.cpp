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

class injectable_super : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE injectable_super() {}
	virtual ~injectable_super() {}

};

class injectable_sub : public injectable_super
{
	Q_OBJECT

public:
	Q_INVOKABLE injectable_sub() {}
	virtual ~injectable_sub() {}

};

class super_dependnecies : public QObject
{
	Q_OBJECT

public:
	injectable_super *v = nullptr;

	Q_INVOKABLE super_dependnecies() {}
	virtual ~super_dependnecies() {}

private slots:
	INJEQT_SET void set_injectable_super(injectable_super *x) { v = x; }

};

class sub_dependnecies : public QObject
{
	Q_OBJECT

public:
	injectable_sub *v = nullptr;

	Q_INVOKABLE sub_dependnecies() {}
	virtual ~sub_dependnecies() {}

private slots:
	INJEQT_SET void set_injectable_sub(injectable_sub *x) { v = x; }

};

class super_sub_dependency_test : public QObject
{
	Q_OBJECT

private:
	injeqt::injector get_injector() const;

private slots:
	void should_set_valid_super_dependencies();
	void should_set_valid_sub_dependencies();
	void should_fail();

};

injeqt::injector super_sub_dependency_test::get_injector() const
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<injectable_sub>();
			add_type<super_dependnecies>();
			add_type<sub_dependnecies>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	return injeqt::injector{std::move(modules)};
}

void super_sub_dependency_test::should_set_valid_super_dependencies()
{
	auto injector = get_injector();
	auto c = injector.get<super_dependnecies>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(c->v == super);
	QVERIFY(c->v == sub);
}

void super_sub_dependency_test::should_set_valid_sub_dependencies()
{
	auto injector = get_injector();
	auto c = injector.get<sub_dependnecies>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(c->v == super);
	QVERIFY(c->v == sub);
}

void super_sub_dependency_test::should_fail()
{
	auto injector = get_injector();
	auto c = injector.get<super_dependnecies>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(c->v == super);;
}

QTEST_APPLESS_MAIN(super_sub_dependency_test)
#include "super-sub-dependency-test.moc"
