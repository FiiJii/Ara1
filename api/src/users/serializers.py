from rest_framework import serializers
from django.contrib.auth.models import User, Permission


class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('id','url', 'username', 'password', 'first_name', 'last_name', 'email', 'is_staff', 'is_superuser', 'is_active')
        extra_kwargs = {'password': {'write_only': True}}
        read_only_fields = ( 'date_joined',)

class PasswordSerializer(serializers.Serializer):
    
    new_password = serializers.CharField(required=True)

