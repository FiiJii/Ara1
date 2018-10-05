from rest_framework import serializers
from rest_framework.validators import UniqueValidator
from django.contrib.auth.models import User

class UserSerializer(serializers.HyperlinkedModelSerializer):
    
    email = serializers.EmailField(
            required=True,
            validators=[UniqueValidator(queryset=User.objects.all())]
            )

    username = serializers.CharField(
            required=True,
            validators=[UniqueValidator(queryset=User.objects.all())]
            )

    password = serializers.CharField(required=True, min_length=8)

    class Meta:
        model = User
        fields = ('id','url', 'username', 'password', 'first_name', 'last_name', 'email', 'is_staff', 'is_superuser', 'is_active')
        extra_kwargs = {'password': {'write_only': True}}
        read_only_fields = ( 'date_joined',)

class PasswordSerializer(serializers.Serializer):
    
    new_password = serializers.CharField(required=True, min_length=8)

