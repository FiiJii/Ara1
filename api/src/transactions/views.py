from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer
from django_filters.rest_framework import DjangoFilterBackend
from transactions.filters import TransactionDateRangeFilter
from rest_framework.decorators import list_route
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import status


class TransactionView(viewsets.ModelViewSet):
    queryset = Transaction.objects.all().order_by('-creation_date')
    serializer_class = TransactionSerializer
    filterset_class = TransactionDateRangeFilter

    @list_route(methods=['get'])
    def max_earnings(self,request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__gt=0)).order_by('-earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)
    
    @list_route(methods=['get'])
    def max_lost(self, request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__lt=0)).order_by('earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)

class TransactionDetailView(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    serializer_class = TransactionDetailSerializer
   
