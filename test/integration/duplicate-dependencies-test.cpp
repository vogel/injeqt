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

class same_duplicate_dependnecies : public QObject
{
	Q_OBJECT

public:
	injectable_super *v1 = nullptr;
	injectable_super *v2 = nullptr;

	Q_INVOKABLE same_duplicate_dependnecies() {}
	virtual ~same_duplicate_dependnecies() {}

private slots:
	INJEQT_SET void set_injectable_super_1(injectable_super *x) { v1 = x; }
	INJEQT_SET void set_injectable_super_2(injectable_super *x) { v2 = x; }

};

class hierarchy_duplicate_dependnecies : public QObject
{
	Q_OBJECT

public:
	injectable_super *v1 = nullptr;
	injectable_sub *v2 = nullptr;

	Q_INVOKABLE hierarchy_duplicate_dependnecies() {}
	virtual ~hierarchy_duplicate_dependnecies() {}

private slots:
	INJEQT_SET void set_injectable_super(injectable_super *x) { v1 = x; }
	INJEQT_SET void set_injectable_sub(injectable_sub *x) { v2 = x; }

};

class same_duplicate_dependnecies_super : public QObject
{
	Q_OBJECT

public:
	injectable_super *v1 = nullptr;

	Q_INVOKABLE same_duplicate_dependnecies_super() {}
	virtual ~same_duplicate_dependnecies_super() {}

private slots:
	INJEQT_SET void set_injectable_super(injectable_super *x) { v1 = x; }

};

class same_duplicate_dependnecies_sub : public same_duplicate_dependnecies_super
{
	Q_OBJECT

public:
	injectable_super *v2 = nullptr;

	Q_INVOKABLE same_duplicate_dependnecies_sub() {}
	virtual ~same_duplicate_dependnecies_sub() {}

private slots:
	INJEQT_SET void set_injectable_super(injectable_super *x) { v2 = x; }

};

class hierarchy_duplicate_dependnecies_super : public QObject
{
	Q_OBJECT

public:
	injectable_super *v1 = nullptr;

	Q_INVOKABLE hierarchy_duplicate_dependnecies_super() {}
	virtual ~hierarchy_duplicate_dependnecies_super() {}

private slots:
	INJEQT_SET void set_injectable_super(injectable_super *x) { v1 = x; }

};

class hierarchy_duplicate_dependnecies_sub : public same_duplicate_dependnecies_super
{
	Q_OBJECT

public:
	injectable_sub *v2 = nullptr;

	Q_INVOKABLE hierarchy_duplicate_dependnecies_sub() {}
	virtual ~hierarchy_duplicate_dependnecies_sub() {}

private slots:
	INJEQT_SET void set_injectable_sub(injectable_sub *x) { v2 = x; }

};

class duplicate_dependencies_test : public QObject
{
	Q_OBJECT

private:
	injeqt::injector get_injector() const;

private slots:
	void should_set_same_duplicate_dependencies_in_class();
	void should_set_hierarchy_duplicate_dependencies_in_class();
	void should_set_same_duplicate_dependencies_in_hierarchy();
	void should_set_hierarchy_duplicate_dependencies_in_hierarchy();

};

injeqt::injector duplicate_dependencies_test::get_injector() const
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<injectable_sub>();
			add_type<same_duplicate_dependnecies>();
			add_type<hierarchy_duplicate_dependnecies>();
			add_type<same_duplicate_dependnecies_sub>();
			add_type<hierarchy_duplicate_dependnecies_sub>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	return injeqt::injector{std::move(modules)};
}

void duplicate_dependencies_test::should_set_same_duplicate_dependencies_in_class()
{
	auto injector = get_injector();
	auto c = injector.get<same_duplicate_dependnecies>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(super == sub);
	QVERIFY(c->v1 == sub);
	QVERIFY(c->v2 == sub);
}

void duplicate_dependencies_test::should_set_hierarchy_duplicate_dependencies_in_class()
{
	auto injector = get_injector();
	auto c = injector.get<hierarchy_duplicate_dependnecies>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(super == sub);
	QVERIFY(c->v1 == sub);
	QVERIFY(c->v2 == sub);
}

void duplicate_dependencies_test::should_set_same_duplicate_dependencies_in_hierarchy()
{
	auto injector = get_injector();
	auto c = injector.get<same_duplicate_dependnecies_sub>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(super == sub);
	QVERIFY(c->v1 == sub);
	QVERIFY(c->v2 == sub);
}

void duplicate_dependencies_test::should_set_hierarchy_duplicate_dependencies_in_hierarchy()
{
	auto injector = get_injector();
	auto c = injector.get<hierarchy_duplicate_dependnecies_sub>();
	auto sub = injector.get<injectable_sub>();
	auto super = injector.get<injectable_super>();

	QVERIFY(c != nullptr);
	QVERIFY(sub != nullptr);
	QVERIFY(super != nullptr);
	QVERIFY(super == sub);
	QVERIFY(c->v1 == sub);
	QVERIFY(c->v2 == sub);
}

QTEST_APPLESS_MAIN(duplicate_dependencies_test)
#include "duplicate-dependencies-test.moc"
