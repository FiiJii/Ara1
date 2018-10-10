from rest_framework.response import Response
from rest_framework import pagination

class OptionalPagination(pagination.PageNumberPagination):
    
    def __init__(self):
        self.use_pagination=True
    
    def paginate_queryset(self, queryset, request, view=None):
        if request.query_params.get("paginate","true")=="false":
            self.use_pagination=False
            return queryset
        else:
            self.use_pagination=True
            return super().paginate_queryset(queryset,request,view)

    def get_paginated_response(self, data):
        if self.use_pagination:
            return super().get_paginated_response(data)
        else:
            return Response(data);