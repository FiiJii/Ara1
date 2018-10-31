from configuration.models import *
from rest_framework.filters import BaseFilterBackend
from django_filters import CharFilter
from functools import reduce
import operator
from django.db.models import Q



class CoinsFilterBackend(BaseFilterBackend):

    def filter_queryset(self, request,queryset,view):
        if "coins" not in request.query_params:
            return
        value=request.query_params["coins"]
        q_list= []
        for v in value.split(','):
          
            q_list.append(Q(symbol__endswith = v))
        qs = queryset.filter(reduce(operator.or_, q_list))
        return qs
