from transactions.models import *
from django_filters import rest_framework as filters
from django_filters import DateRangeFilter, DateTimeFilter, IsoDateTimeFilter


class MultiValueCharFilter(filters.BaseCSVFilter, filters.CharFilter):
    def filter(self, qs, value):
        # value is either a list or an 'empty' value
        values = value or []

        for value in values:
            qs = super(MultiValueCharFilter, self).filter(qs, value)

        return qs

class CurrencyFilter(filters.FilterSet):
    currency = MultiValueCharFilter(field_name='parity', lookup_expr='contains')

    class Meta:
        model = TransactionDetail
        fields = ['currency',]


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
