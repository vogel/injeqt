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

#include "implements-extractor.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

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

public slots:
	injeqt_setter void tagged_setter_1(injectable_type1 *a) { _1 = a; }
	injeqt_setter void tagged_int_setter(int) {}
	injeqt_setter void tagged_invalid_setter(injectable_type1 *, injectable_type2 *) {}
	void untagged_setter_1(injectable_type1 *) {}
	void untagged_int_setter(int) {}
	void untagged_invalid_setter(injectable_type1 *, injectable_type2 *) {}

};

class setter_method_test : public QObject
{
	Q_OBJECT

public:
	setter_method_test();

private slots:
	void should_throw_when_created_with_empty_method();
	void should_create_valid_from_tagged_setter();
	void should_create_valid_from_untagged_setter();
	void should_throw_when_created_from_tagged_int_setter();
	void should_throw_when_created_from_untagged_int_setter();
	void should_throw_when_created_from_tagged_invalid_setter();
	void should_throw_when_created_from_untagged_invalid_setter();
	void should_throw_when_called_on_null_object();
	void should_throw_when_called_on_wrong_object();
	void should_throw_when_called_with_null_object();
	void should_throw_when_called_with_wrong_object();
	void should_invoke_have_results();

private:
	type injectable_type1_type;
	type injectable_type2_type;
	type test_type_type;

};

setter_method_test::setter_method_test() :
	injectable_type1_type{std::addressof(injectable_type1::staticMetaObject)},
	injectable_type2_type{std::addressof(injectable_type2::staticMetaObject)},
	test_type_type{std::addressof(test_type::staticMetaObject)}
{
}

void setter_method_test::should_throw_when_created_with_empty_method()
{
	expect<invalid_setter_exception>([]{
		auto setter = setter_method{{}};
	});
}

void setter_method_test::should_create_valid_from_tagged_setter()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};

	QCOMPARE(setter.object_type(), test_type_type);
	QCOMPARE(setter.parameter_type(), injectable_type1_type);
}

void setter_method_test::should_create_valid_from_untagged_setter()
{
	auto setter = setter_method{method<test_type>("untagged_setter_1(injectable_type1*)")};

	QCOMPARE(setter.object_type(), test_type_type);
	QCOMPARE(setter.parameter_type(), injectable_type1_type);
}

void setter_method_test::should_throw_when_created_from_tagged_int_setter()
{
	expect<invalid_setter_exception>([&]{
		auto setter = setter_method{method<test_type>("tagged_int_setter(int)")};
	});
}

void setter_method_test::should_throw_when_created_from_untagged_int_setter()
{
	expect<invalid_setter_exception>([&]{
		auto setter = setter_method{method<test_type>("untagged_int_setter(int)")};
	});
}

void setter_method_test::should_throw_when_created_from_tagged_invalid_setter()
{
	expect<setter_too_many_parameters_exception>([&]{
		auto setter = setter_method{method<test_type>("tagged_invalid_setter(injectable_type1*,injectable_type2*)")};
	});
}

void setter_method_test::should_throw_when_created_from_untagged_invalid_setter()
{
	expect<setter_too_many_parameters_exception>([&]{
		auto setter = setter_method{method<test_type>("untagged_invalid_setter(injectable_type1*,injectable_type2*)")};
	});
}

void setter_method_test::should_throw_when_called_on_null_object()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};
	auto with = make_object<injectable_type1>();
	expect<invoked_on_wrong_object_exception>([&]{
		setter.invoke(nullptr, with.get());
	});
}

void setter_method_test::should_throw_when_called_on_wrong_object()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};
	auto on = make_object<injectable_type1>();
	auto with = make_object<injectable_type1>();
	expect<invoked_on_wrong_object_exception>([&]{
		setter.invoke(on.get(), with.get());
	});
}

void setter_method_test::should_throw_when_called_with_null_object()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};
	auto on = make_object<injectable_type1>();
	expect<invoked_on_wrong_object_exception>([&]{
		setter.invoke(on.get(), nullptr);
	});
}

void setter_method_test::should_throw_when_called_with_wrong_object()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};
	auto on = make_object<injectable_type1>();
	auto with = make_object<test_type>();
	expect<invoked_on_wrong_object_exception>([&]{
		setter.invoke(on.get(), with.get());
	});
}

void setter_method_test::should_invoke_have_results()
{
	auto setter = setter_method{method<test_type>("tagged_setter_1(injectable_type1*)")};
	auto on = make_object<test_type>();
	auto with = make_object<injectable_type1>();

	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<test_type *>(on.get())->_1);

	setter.invoke(on.get(), with.get());

	QCOMPARE(with.get(), static_cast<test_type *>(on.get())->_1);
}

QTEST_APPLESS_MAIN(setter_method_test)
#include "setter-method-test.moc"
