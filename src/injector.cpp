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

#include "injector.h"

namespace injeqt { namespace v1 {

injector::injector(std::vector<module *> modules) :
	_modules{std::move(modules)}
{

}

factory/creator?

injeqt::item
	QMetaObject *itemType
	QObject *object
	bool initialized (all subobjects set)

injeqt::item_dependencies
	QMetaObject *itemType
	std::vector<QMetaObject *> dependents

	-> use graph from kadu
		what about bidirectional dependencies?
		can be done, but not for factory

dependency_extrator
	QMetaObject
		set ?
		tag ?
		optional/not optional?

injeqt::internal::factory

injeqt::internal::existing_item_factory
	::create<> return item

injeqt::internal::type_item_factory
	::create<> return type->create

injeqt::internal::factory-item_factory
	::create<> return factory->create // ale factory musi być zrobione i zresolvovane!

QObject * injector::instance(const QMetaObject &itemType)
{
	auto created = _instances.find(&itemType);
	if (created != std::end(_instances))
		return created->second;

	for (auto module : _modules)
	{
		auto item = module->item(itemType);
		if (item)
	}
}

}}
