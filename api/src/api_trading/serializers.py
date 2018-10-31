from rest_framework.utils.field_mapping import (
    ClassLookupDict, get_field_kwargs, get_nested_relation_kwargs,
    get_relation_kwargs, get_url_kwargs
)
from rest_framework import serializers
"""
 Serializer Class that allow setting the id for related fields with the deep parameter set 
 -Can`t edit the related field data
 -To specify the SerializerClass to on related fields  field nested_classes on Meta 
    example:
        nested_classes={"field_name",SerializerClass}
"""
class ExpandableModelSerializer(serializers.HyperlinkedModelSerializer):

    def build_field(self, field_name, info, model_class, nested_depth):
        """
        Return a two tuple of (cls, kwargs) to build a serializer field with.
        """
        if field_name in info.fields_and_pk:
            model_field = info.fields_and_pk[field_name]
            return self.build_standard_field(field_name, model_field)

        elif field_name in info.relations:
            relation_info = info.relations[field_name]
            if not nested_depth or self.context["request"].method in ["PUT","POST","PATCH"]:
                return self.build_relational_field(field_name, relation_info)
            else:
                return self.build_nested_field(field_name, relation_info, nested_depth)

        elif hasattr(model_class, field_name):
            return self.build_property_field(field_name, model_class)

        elif field_name == self.url_field_name:
            return self.build_url_field(field_name, model_class)

        return self.build_unknown_field(field_name, model_class)


    def build_nested_field(self, field_name, relation_info, nested_depth):
        """
        Create nested fields for forward and reverse relationships.
        """
        meta = getattr(self, 'Meta', None)
        base_class=meta.nested_classes[field_name] if field_name in meta.nested_classes else ModelSerializer
        print (base_class)
        class NestedSerializer(base_class):
            class Meta:
                model = relation_info.related_model
                depth = nested_depth - 1
                fields = '__all__'
        
        field_class = NestedSerializer
        field_kwargs = get_nested_relation_kwargs(relation_info)

        return field_class, field_kwargs


