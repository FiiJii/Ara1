from transactions.models import *
from django_filters import rest_framework as filters
from django_filters import DateRangeFilter, DateTimeFilter, IsoDateTimeFilter, CharFilter
from functools import reduce
import operator
from django.db.models import Q

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


class CurrencyFilter(filters.FilterSet):
    currency = CharFilter(method ='my_custom_filter')

    class Meta:
        model = TransactionDetail
        fields = ['currency']

    def my_custom_filter(self, queryset, name, value):

        q_list= []

        for v in value.split(','):
          
            q_list.append(Q(parity__endswith = v))

            #q_list = [Q(parity__endswith = v), Q(parity__endswith = v)]
            
        qs = queryset.filter(reduce(operator.or_, q_list))
                        
        return qs