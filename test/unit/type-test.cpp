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
#include "type.cpp"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_test : public QObject
{
	Q_OBJECT

private slots:
	void should_throw_when_created_with_null_meta_object();
	void should_retun_valid_meta_object();

};

class valid_type : public QObject
{
	Q_OBJECT
};

void type_test::should_retun_valid_meta_object()
{
	expect<invalid_type_exception>([]{
		auto t = type{nullptr};
	});
}

void type_test::should_throw_when_created_with_null_meta_object()
{
	auto t = make_type<valid_type>();

	QCOMPARE(t.meta_object(), &valid_type::staticMetaObject);
}

QTEST_APPLESS_MAIN(type_test)
#include "type-test.moc"
