package zabavy.zabavyhome;

public class itemLista {

    private int Imagen;
    private String Nombre;
    private String Descripcion;
    private String ID;
    private boolean Estado;
    private int Color;
    private int Cantidad;

    public itemLista(int Imagen, String Nombre, String Descripcion, String ID, boolean Estado, int Color, int Cantidad){
        this.Imagen = Imagen;
        this.Nombre = Nombre;
        this.Descripcion = Descripcion;
        this.ID = ID;
        this.Estado = Estado;
        this.Color = Color;
        this.Cantidad = Cantidad;
    }

    public int getImagen(){
        return Imagen;
    }

    public String getNombre(){
        return Nombre;
    }

    public String getDescripcion(){
        return Descripcion;
    }

    public String getID(){
        return ID;
    }

    public boolean getEstado(){
        return Estado;
    }

    public int getColor(){
        return Color;
    }

    public int getCantidad(){
        return Cantidad;
    }
}
