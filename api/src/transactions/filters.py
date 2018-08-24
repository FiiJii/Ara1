from transactions.models import Transaction, TransactionDetail
from django_filters import rest_framework as filters
from django_filters import DateRangeFilter, DateTimeFilter, IsoDateTimeFilter


class TransactionDateFilter(filters.FilterSet):
    
    class Meta:
        model = Transaction
        fields = {
            'creation_date': ['exact', 'lte', 'gte']
        }

class TransactionDateRangeFilter(filters.FilterSet):
    start_date = IsoDateTimeFilter(field_name='creation_date', lookup_expr='gte') 
    end_date = IsoDateTimeFilter(field_name='creation_date', lookup_expr='lte')
    date_range = DateRangeFilter(field_name='creation_date')

    class Meta:
        model = Transaction
        fields = ['date_range', 'start_date', 'end_date']