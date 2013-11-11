/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG)

#include "JSSVGPathSegCurvetoCubicAbs.h"

#include "SVGPathSegCurvetoCubic.h"
#include <runtime/JSNumberCell.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGPathSegCurvetoCubicAbs);

/* Hash table */

static const HashTableValue JSSVGPathSegCurvetoCubicAbsTableValues[8] =
{
    { "x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX },
    { "y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY },
    { "x1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX1), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX1 },
    { "y1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY1), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY1 },
    { "x2", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX2), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX2 },
    { "y2", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY2), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY2 },
    { "constructor", DontEnum|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsConstructor), (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSSVGPathSegCurvetoCubicAbsTableValues, 0 };
#else
    { 16, 15, JSSVGPathSegCurvetoCubicAbsTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSSVGPathSegCurvetoCubicAbsConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGPathSegCurvetoCubicAbsConstructorTableValues, 0 };
#else
    { 1, 0, JSSVGPathSegCurvetoCubicAbsConstructorTableValues, 0 };
#endif

class JSSVGPathSegCurvetoCubicAbsConstructor : public DOMConstructorObject {
public:
    JSSVGPathSegCurvetoCubicAbsConstructor(ExecState* exec, JSDOMGlobalObject* globalObject)
        : DOMConstructorObject(JSSVGPathSegCurvetoCubicAbsConstructor::createStructure(globalObject->objectPrototype()), globalObject)
    {
        putDirect(exec->propertyNames().prototype, JSSVGPathSegCurvetoCubicAbsPrototype::self(exec, globalObject), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValue proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
    }
    
protected:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSSVGPathSegCurvetoCubicAbsConstructor::s_info = { "SVGPathSegCurvetoCubicAbsConstructor", 0, &JSSVGPathSegCurvetoCubicAbsConstructorTable, 0 };

bool JSSVGPathSegCurvetoCubicAbsConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoCubicAbsConstructor, DOMObject>(exec, &JSSVGPathSegCurvetoCubicAbsConstructorTable, this, propertyName, slot);
}

bool JSSVGPathSegCurvetoCubicAbsConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegCurvetoCubicAbsConstructor, DOMObject>(exec, &JSSVGPathSegCurvetoCubicAbsConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegCurvetoCubicAbsPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGPathSegCurvetoCubicAbsPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGPathSegCurvetoCubicAbsPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGPathSegCurvetoCubicAbsPrototype::s_info = { "SVGPathSegCurvetoCubicAbsPrototype", 0, &JSSVGPathSegCurvetoCubicAbsPrototypeTable, 0 };

JSObject* JSSVGPathSegCurvetoCubicAbsPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSegCurvetoCubicAbs>(exec, globalObject);
}

const ClassInfo JSSVGPathSegCurvetoCubicAbs::s_info = { "SVGPathSegCurvetoCubicAbs", &JSSVGPathSeg::s_info, &JSSVGPathSegCurvetoCubicAbsTable, 0 };

JSSVGPathSegCurvetoCubicAbs::JSSVGPathSegCurvetoCubicAbs(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPathSegCurvetoCubicAbs> impl)
    : JSSVGPathSeg(structure, globalObject, impl)
{
}

JSObject* JSSVGPathSegCurvetoCubicAbs::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSSVGPathSegCurvetoCubicAbsPrototype(JSSVGPathSegCurvetoCubicAbsPrototype::createStructure(JSSVGPathSegPrototype::self(exec, globalObject)));
}

bool JSSVGPathSegCurvetoCubicAbs::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoCubicAbs, Base>(exec, &JSSVGPathSegCurvetoCubicAbsTable, this, propertyName, slot);
}

bool JSSVGPathSegCurvetoCubicAbs::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegCurvetoCubicAbs, Base>(exec, &JSSVGPathSegCurvetoCubicAbsTable, this, propertyName, descriptor);
}

JSValue jsSVGPathSegCurvetoCubicAbsX(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->x());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsY(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->y());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsX1(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->x1());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsY1(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->y1());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsX2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->x2());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsY2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->y2());
    return result;
}

JSValue jsSVGPathSegCurvetoCubicAbsConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* domObject = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    return JSSVGPathSegCurvetoCubicAbs::getConstructor(exec, domObject->globalObject());
}
void JSSVGPathSegCurvetoCubicAbs::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    lookupPut<JSSVGPathSegCurvetoCubicAbs, Base>(exec, propertyName, value, &JSSVGPathSegCurvetoCubicAbsTable, this, slot);
}

void setJSSVGPathSegCurvetoCubicAbsX(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setX(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

void setJSSVGPathSegCurvetoCubicAbsY(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setY(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

void setJSSVGPathSegCurvetoCubicAbsX1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setX1(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

void setJSSVGPathSegCurvetoCubicAbsY1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setY1(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

void setJSSVGPathSegCurvetoCubicAbsX2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setX2(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

void setJSSVGPathSegCurvetoCubicAbsY2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThisObj = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThisObj->impl());
    imp->setY2(value.toFloat(exec));
    JSSVGContextCache::propagateSVGDOMChange(castedThisObj, imp->associatedAttributeName());
}

JSValue JSSVGPathSegCurvetoCubicAbs::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPathSegCurvetoCubicAbsConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG)
