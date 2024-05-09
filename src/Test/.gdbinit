
#add-auto-load-safe-path $POND_ROOT/etc/gdb/pond

set auto-load local-gdbinit
set overload-resolution on
set print object on
set print pretty on
set unwindonsignal on

python

#import sys,os
#sys.path.insert(0, os.path.join(os.environ.get('POND_ROOT'), 'etc/gdb/stlprettyprinter') )
#from libstdcxx.v6.printers import register_libstdcxx_printers
#register_libstdcxx_printers (None)

import gdb
max_matrix_row = 10
print("pond gdbinit script is sourced")

##########################
class IndexPrinter(object):
    "Print a pond::Index"
    def __init__(self,val):
        #print('init index printer',val)
        self.val = val
        pass
    def to_string(self):
        #print("try get string of Index")
        return "$$_"+str(self.val['row'])+'&'+str(self.val['col'])

    def display_hint(self):
        return "pond::Index"

class ComplexNumberPrinter(object):
    "Print pond::complex"
    def __init__(self, val):
        self.val = val
        pass
    def to_string(self):
        return "("+str(self.val['re'])+","+str(self.val['im'])+")"
        pass
    def display_hint(self):
        return "pond::complex"
        pass
    pass

def object2string(obj):
    if obj:
        #print( 'objid = ',obj['objid']['row'], obj['objid']['row'] )
        #if obj['objid']['row'] == 0 and obj['objid']['row'] == 0: 
        #    return "Null"
        if obj['objid']['row'] > 4194954895: 
            return "$$uninitialized$$"
        try:
            _type = str( gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->type()' ) )

            if _type == "pond::ObjectType::Number":
                _val = str( gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->Number()' ) )
                return "{0}".format(_val )

            if _type == "pond::ObjectType::String":
                _val = ( gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->Key()' ) ).string()
                return "\"{0}\"".format( _val )

            if _type == "pond::ObjectType::Symbol":
                _val = (gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->Key()' ) ).string()
                return "{0}".format( _val )

            if _type == "pond::ObjectType::List":
                _size = int(gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->ElementsSize()' ))
                if _size == 0: 
                    return "pond::ObjectType::List = Null"

                head = object2string( gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->ElementAt(0)' ) )
                cont       =  head+"("
                end_ch     = ')'
                if head == 'List':
                    cont      = "["
                    end_ch    = "]"
                    pass

                for i in range(1, _size ):
                    v     = gdb.parse_and_eval( '((pond::Object*)'+str( obj.address )+')->ElementAt('+str(i)+')'  )
                    cont += object2string( v )
                    if i != _size-1: cont += ', '
                    pass
                cont += end_ch
                
                return cont

            return "pond::UnknownObject"

        except Exception as e:
            print("get object string failed for:",e)
            return "failed object string"

    return "pond::Object val"


class ObjectPrinter(object):
    "Print a pond::Object"
    def __init__(self,val):
        self.val = val
        pass

    def to_string(self):
        return object2string(self.val)

    def display_hint(self):
        return "pond::Object"

    pass

class MatrixPrinter(object):
    "Print a pond::Matrix"
    def __init__(self,val):
        self.val = val
        pass

    def to_string(self):
        try:
            #print( self.val )
            d = [0,0,0,0,0,0,0,0,0,0,0]
            d[0] = int( self.val['ND'] )
            dim = '{\n  ND = '+str( d[0] ) 
            if d[0] == 0:
                return "pond::Matrix = null"
            d[1] = int( self.val["D1"] )
            size = d[1]
            dim += ", D1 = " + str(d[1])
            for i in range(2, min(9,d[0]+1) ):
                d[i] = int( self.val["D"+str(i)] )
                size *= d[i]
                dim += ", D"+str(i)+" = "+str( d[i] )
                pass
            mat_vals = dim + ", \n"
            mat_vals += "  DataDevice = {0},\n".format(str(self.val['DataDevice']) )
            mat_vals += "  Data = {0}\n".format(str(self.val['Data']) )
            mat_vals += "}"
            data = self.val['Data']
            mat = "\nData = [\n"
            s1 = min( max_matrix_row, d[1])
            s2 = min( max_matrix_row, d[2] )
            s3 = min( 3, d[3] ) 
            s4 = min( 3, d[4] ) 
            s5 = min( 3, d[5] ) 
            s6 = min( 3, d[6] ) 
            s7 = min( 3, d[7] ) 
            s8 = min( 3, d[8] ) 
            if d[2] == 0: mat += "  "
            for i in range( s1 ):
                if s2 == 0:
                    mat += str( data[i] )
                else:
                    mat += "  ["
                    for j in range( s2 ):
                        if s3 == 0:
                            mat += str(data[ i*d[2]+j ]) 
                        else:
                            mat += " ["
                            for k in range( s3 ):
                                if s4 == 0:
                                    mat += str( data[ (d[2]*i+j)*d[3]+k] )
                                else:
                                    mat += " ["
                                    for l in range( s4 ):
                                        if s5 == 0:
                                            mat += str( data[ ((d[2]*i+j)*d[3]+k)*d[4]+l ] )
                                        else:
                                            mat += " ["
                                            for m in range( s5 ):
                                                if s6 == 0:
                                                    mat += str( data[ (((d[2]*i+j)*d[3]+k)*d[4]+l)*d[5]+m ] )
                                                else:
                                                    mat += " ["
                                                    for n in range( s6 ):
                                                        if s7 == 0:
                                                            mat += str( data[ ((((d[2]*i+j)*d[3]+k)*d[4]+l)*d[5]+m)*d[6]+n ] )
                                                        else:
                                                            mat += " ["
                                                            for o in range( s7 ):
                                                                if s8 == 0:
                                                                    mat += str( data[ (((((d[2]*i+j)*d[3]+k)*d[4]+l)*d[5]+m)*d[6]+n)*d[7]+o ] )
                                                                else:
                                                                    mat += " ["
                                                                    for p in range(s8):
                                                                        mat += str( data[ ((((((d[2]*i+j)*d[3]+k)*d[4]+l)*d[5]+m)*d[6]+n)*d[7]+o)*d[8]+p ] )
                                                                        if p < d[8]-1: mat += ", "
                                                                    if s8 < d[8]: mat += "..."
                                                                    mat += "]"
                                                                    pass
                                                                if o < d[7]-1: mat += ", "
                                                            if s7 < d[7]: mat += "..."
                                                            mat += "]"
                                                            pass
                                                        if n < d[6]-1: mat += ", "
                                                    if s6 < d[6]: mat += "..."
                                                    mat += "]"
                                                    pass
                                                if m < d[5]-1: mat += ", "
                                            if s5 < d[5]: mat += "..."
                                            mat += "]"
                                            pass
                                        if l < d[4]-1: mat += ", "
                                        pass
                                    if s4 < d[4]: mat += "..."
                                    mat += "]"
                                    pass
                                if k < d[3]-1: mat += ", "
                                pass
                            if s3 < d[3]: mat += "..."
                            mat += "]"
                            pass
                        if j < d[2]-1: mat += ", "
                        pass
                    if s2 < d[2]: mat += "..."
                    mat += "]"
                    pass
                if i < d[1]-1: mat += ", "
                if d[2] != 0: mat += "\n"
                pass
            if s1 < d[1]: mat += "  ..."
            mat += "\n]"
            return mat_vals + mat
        except Exception as e:
            print("get matrix value failed:", e)
            return "Null Matrix"
        pass

    def display_hint(self):
        return "pond::Matrix"
    pass


            

import gdb.printing
def build_pretty_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("pond")
    pp.add_printer('Index',"^pond::Index$",IndexPrinter)
    pp.add_printer('complex',"^pond::complex$",ComplexNumberPrinter)
    pp.add_printer('floatcomplex',"^pond::floatcomplex$",ComplexNumberPrinter)
    pp.add_printer('Object',"^pond::Object$",ObjectPrinter)
    pp.add_printer('Object',"^pond::Matrix",MatrixPrinter)
    #pp.add_printer('Object',"^pond::Matrix_T$",MatrixPrinter)
    return pp

gdb.printing.register_pretty_printer(
    gdb.current_objfile(),
    build_pretty_printers()
)


end
