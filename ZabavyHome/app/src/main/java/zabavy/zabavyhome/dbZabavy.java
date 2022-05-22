package zabavy.zabavyhome;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class dbZabavy extends SQLiteOpenHelper{

    public dbZabavy(Context context) {
        super(context, "Zabavy.db", null, 1);
    }

    public void onCreate(SQLiteDatabase db){
        db.execSQL("CREATE TABLE ZABAVY ('ID' INTEGER PRIMARY KEY AUTOINCREMENT, 'IMAGEN' INTEGER DEFAULT " + R.drawable.lights64 + ", 'NOMBRE' VARCHAR(50) NOT NULL, 'DESCRIPCION' TEXT, 'DIRECCION' VARCHAR(15) NOT NULL, 'ESTADO' BIT NOT NULL DEFAULT 0, 'COLOR' INTEGER NOT NULL DEFAULT 0, 'CANTIDAD' INTEGER NOT NULL DEFAULT 0);");
    }

    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){

    }

    public String[] cargarDB(int posicion){
        SQLiteDatabase db = this.getReadableDatabase();
        String[] projection = {"ID", "IMAGEN", "NOMBRE", "DESCRIPCION", "DIRECCION", "ESTADO", "COLOR", "CANTIDAD"};
        String[] datos = new String[]{null, null, null, null, null, null, null, null};
        Cursor cursor = db.query("ZABAVY", projection, null, null, null, null, null, null);
        if (cursor != null) {
            cursor.moveToPosition(posicion);
            datos[0] = cursor.getString(0);
            datos[1] = cursor.getString(1);
            datos[2] = cursor.getString(2);
            datos[3] = cursor.getString(3);
            datos[4] = cursor.getString(4);
            datos[5] = cursor.getString(5);
            datos[6] = cursor.getString(6);
            datos[7] = cursor.getString(7);
        }
        db.close();
        return datos;
    }

    public long insertDB(int Imagen, String Nombre, String Descripcion, String Direccion, boolean Estado, int Color){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put("IMAGEN", Imagen);
        values.put("NOMBRE", Nombre);
        values.put("DESCRIPCION", Descripcion);
        values.put("DIRECCION", Direccion);
        values.put("ESTADO", Estado);
        values.put("COLOR", Color);
        long newRowId = db.insert("ZABAVY", null, values);
        db.close();
        return newRowId;
    }

    public String[] readDB(long id){
        SQLiteDatabase db = this.getReadableDatabase();
        String[] projection = {"ID", "IMAGEN", "NOMBRE", "DESCRIPCION", "DIRECCION", "ESTADO", "COLOR", "CANTIDAD"};
        String[] datos = new String[]{null, null, null, null, null, null, null, null};
        Cursor cursor = db.query("ZABAVY", projection, "ID = ?", new String[]{String.valueOf(id)}, null, null, null, null);
        if (cursor != null) {
            cursor.moveToFirst();
            datos[0] = cursor.getString(0);
            datos[1] = cursor.getString(1);
            datos[2] = cursor.getString(2);
            datos[3] = cursor.getString(3);
            datos[4] = cursor.getString(4);
            datos[5] = cursor.getString(5);
            datos[6] = cursor.getString(6);
            datos[7] = cursor.getString(7);
        }
        db.close();
        return datos;
    }

    public int getCount(){
        int count = 0;
        SQLiteDatabase db = this.getReadableDatabase();
        String[] projection = {"COUNT()"};
        Cursor cursor = db.query("ZABAVY", projection, null, null, null, null, null, null);
        if (cursor != null) {
            cursor.moveToFirst();
            count = cursor.getInt(0);
        }
        db.close();
        return count;
    }

    public void updateDB(int ID, int Imagen, String Nombre, String Descripcion, String Direccion, boolean Estado, int Color, int Cantidad){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put("IMAGEN", Imagen);
        values.put("NOMBRE", Nombre);
        values.put("DESCRIPCION", Descripcion);
        values.put("DIRECCION", Direccion);
        values.put("ESTADO", Estado);
        values.put("COLOR", Color);
        values.put("CANTIDAD", Cantidad);
        db.update("ZABAVY", values, "ID = ?", new String[]{String.valueOf(ID)});
        db.close();
    }

    public boolean deleteDB(int ID) {
        SQLiteDatabase db = this.getWritableDatabase();
        try {
            db.delete("ZABAVY", "ID = ?", new String[]{String.valueOf(ID)});
            db.close();
            return true;
        } catch(Exception ex) {
            return false;
        }
    }

}
