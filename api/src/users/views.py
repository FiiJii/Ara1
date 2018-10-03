from django.contrib.auth.models import User
from rest_framework import viewsets
from rest_framework.permissions import IsAuthenticated, IsAdminUser
from users.serializers import *
from rest_framework.decorators import action
from rest_framework.response import Response
from rest_framework.decorators import list_route

class UserView(viewsets.ModelViewSet):
    queryset = User.objects.all().order_by('-date_joined')
    serializer_class = UserSerializer
    permission_classes = (IsAuthenticated, IsAdminUser)

    @action(methods=['post'], detail=True)
    def set_password(self, request, pk=None):
        user = self.get_object()
        serializer = PasswordSerializer(data=request.data)
        if serializer.is_valid():
            user.set_password(serializer.data['new_password'])
            user.save()
            return Response({'status': 'password set'})
        else:
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)