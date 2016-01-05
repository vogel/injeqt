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

#include "expect.h"
#include "utils.h"

#include <injeqt/exception/invalid-setter.h>

#include "internal/types-by-name.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;
using namespace injeqt::v1;

class injectable_type1 : public QObject
{
	Q_OBJECT
};

class injectable_type2 : public QObject
{
	Q_OBJECT
};

class test_type : public QObject
{
	Q_OBJECT

public:
	injectable_type1 *_1 = nullptr;

	test_type() {}

	Q_INVOKABLE test_type(injectable_type1 *) {}

	Q_INVOKABLE INJEQT_SET void tagged_setter_method_1(injectable_type1 *a) { _1 = a; }
	Q_INVOKABLE INJEQT_SETTER void tagged_setter_method_2(injectable_type1 *a) { _1 = a; }

public slots:
	INJEQT_SET void tagged_setter_slot_1(injectable_type1 *a) { _1 = a; }
	INJEQT_SETTER void tagged_setter_slot_2(injectable_type1 *a) { _1 = a; }
	INJEQT_SETTER void invalid_setter_multi_arguments(injectable_type1 *, injectable_type2 *) { }
	void invalid_setter_no_tag(injectable_type1 *) { }

signals:
	INJEQT_SETTER void invalid_setter_signal(injectable_type1 *);

};

class setter_method_test : public QObject
{
	Q_OBJECT

public:
	setter_method_test();

private:
	types_by_name _known_types;

private slots:
	void should_create_empty();
	void should_create_valid_from_tagged_setter_method();
	void should_create_valid_from_tagged_setter_slot();
	void should_invoke_have_results();
	void should_throw_when_empty_method();
	void should_throw_when_multiple_arguments();
	void should_throw_when_no_tag();
	void should_throw_when_signal();
	void should_throw_when_constructor();
	void should_throw_when_non_matching_parameters();

};

setter_method_test::setter_method_test() :
	_known_types{
		make_type<injectable_type1>(),
		make_type<injectable_type2>(),
		make_type<test_type>()
	}
{
}

void setter_method_test::should_create_empty()
{
	auto setter = setter_method{};
	QVERIFY(setter.is_empty());
}

void setter_method_test::should_create_valid_from_tagged_setter_method()
{
	auto setter = make_setter_method(_known_types, get_method<test_type>("tagged_setter_method_1(injectable_type1*)"));
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());

	setter = make_setter_method(_known_types, get_method<test_type>("tagged_setter_method_2(injectable_type1*)"));
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());
}

void setter_method_test::should_create_valid_from_tagged_setter_slot()
{
	auto setter = make_setter_method(_known_types, get_method<test_type>("tagged_setter_slot_1(injectable_type1*)"));
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());

	setter = make_setter_method(_known_types, get_method<test_type>("tagged_setter_slot_2(injectable_type1*)"));
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());
}

void setter_method_test::should_invoke_have_results()
{
	auto setter = make_setter_method(_known_types, get_method<test_type>("tagged_setter_slot_1(injectable_type1*)"));
	auto on = make_object<test_type>();
	auto with = make_object<injectable_type1>();
	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<test_type *>(on.get())->_1);

	setter.invoke(on.get(), with.get());
	QCOMPARE(with.get(), static_cast<test_type *>(on.get())->_1);
}

void setter_method_test::should_throw_when_empty_method()
{
	expect<exception::invalid_setter>({"setter does not have enclosing meta object"}, [&]{
		setter_method{make_type<injectable_type1>(), QMetaMethod{}};
	});
	expect<exception::invalid_setter>({"setter does not have enclosing meta object"}, [&]{
		make_setter_method(_known_types, QMetaMethod{});
	});
}

void setter_method_test::should_throw_when_multiple_arguments()
{
	expect<exception::invalid_setter>({"invalid parameter count"}, [&]{
		setter_method{make_type<injectable_type1>(), get_method<test_type>("invalid_setter_multi_arguments(injectable_type1*,injectable_type2*)")};
	});
	expect<exception::invalid_setter>({"invalid parameter count"}, [&]{
		make_setter_method(_known_types, get_method<test_type>("invalid_setter_multi_arguments(injectable_type1*,injectable_type2*)"));
	});
}

void setter_method_test::should_throw_when_no_tag()
{
	expect<exception::invalid_setter>({"setter does not have valid tag"}, [&]{
		setter_method{make_type<injectable_type1>(), get_method<test_type>("invalid_setter_no_tag(injectable_type1*)")};
	});
	expect<exception::invalid_setter>({"setter does not have valid tag"}, [&]{
		make_setter_method(_known_types, get_method<test_type>("invalid_setter_no_tag(injectable_type1*)"));
	});
}

void setter_method_test::should_throw_when_signal()
{
	expect<exception::invalid_setter>({"setter is signal"}, [&]{
		setter_method{make_type<injectable_type1>(), get_method<test_type>("invalid_setter_signal(injectable_type1*)")};
	});
	expect<exception::invalid_setter>({"setter is signal"}, [&]{
		make_setter_method(_known_types, get_method<test_type>("invalid_setter_signal(injectable_type1*)"));
	});
}

void setter_method_test::should_throw_when_constructor()
{
	expect<exception::invalid_setter>({"setter is constructor"}, [&]{
		setter_method{make_type<injectable_type1>(), get_constructor<test_type>("test_type(injectable_type1*)")};
	});
	expect<exception::invalid_setter>({"setter is constructor"}, [&]{
		make_setter_method(_known_types, get_constructor<test_type>("test_type(injectable_type1*)"));
	});
}

void setter_method_test::should_throw_when_non_matching_parameters()
{
	expect<exception::invalid_setter>({"invalid parameter"}, [&]{
		setter_method{make_type<injectable_type2>(), get_method<test_type>("tagged_setter_slot_1(injectable_type1*)")};
	});
}

QTEST_APPLESS_MAIN(setter_method_test)
#include "setter-method-test.moc"
