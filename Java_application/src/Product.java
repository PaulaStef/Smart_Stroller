public class Product {
    private Integer id_product;
    private String product_name;
    private Integer quantity;
    private Integer price;

    public Integer getId_product() {
        return id_product;
    }

    public String getProduct_name() {
        return product_name;
    }

    public Integer getQuantity() {
        return quantity;
    }

    public Integer getPrice() {
        return price;
    }

    public void setId_product(Integer id_product) {
        this.id_product = id_product;
    }

    public void setProduct_name(String product_name) {
        this.product_name = product_name;
    }

    public void setQuantity(Integer quantity) {
        this.quantity = quantity;
    }

    public void setPrice(Integer price) {
        this.price = price;
    }
}
