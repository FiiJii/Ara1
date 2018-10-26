from configuration.models import *
from django_filters import rest_framework as filters
from django_filters import CharFilter
from functools import reduce
import operator
from django.db.models import Q



class CurrencyFilter(filters.FilterSet):
    coins = CharFilter(method ='my_custom_filter')

    class Meta:
        model = Currency
        fields = ['coins']

    def my_custom_filter(self, queryset, name, value):

        q_list= []

        for v in value.split(','):
          
            q_list.append(Q(symbol__endswith = v))

            #q_list = [Q(parity__endswith = v), Q(parity__endswith = v)]
            
        qs = queryset.filter(reduce(operator.or_, q_list))
                        
        return qs