from django.contrib.auth.models import User
from rest_framework import viewsets, status
from rest_framework.permissions import IsAuthenticated, IsAdminUser
from users.serializers import *
from rest_framework.decorators import action
from rest_framework.response import Response
from api_trading.pagination import OptionalPagination

class UserView(viewsets.ModelViewSet):
    queryset = User.objects.all().order_by('-date_joined')
    serializer_class = UserSerializer
    permission_classes = (IsAuthenticated, IsAdminUser)
    pagination_class = OptionalPagination

    def perform_create(self, serializer):
        user = serializer.save()
        user.set_password(user.password)
        user.save()

    @action(methods=['post'], detail=True)
    def change_password(self, request, pk=None):
        user = self.get_object()
        serializer = PasswordSerializer(data=request.data)
        if serializer.is_valid():
            user.set_password(serializer.data['new_password'])
            user.save()
            return Response({'status': 'password changed successfully'})
        else:
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)