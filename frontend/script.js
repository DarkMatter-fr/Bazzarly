document.addEventListener("DOMContentLoaded", () => {

let cart = [];

// PAGE SWITCH
function showPage(page) {
  document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
  document.getElementById(page).classList.add('active');

  if (page === 'cart') renderCart();
}
window.showPage = showPage;

// LOAD PRODUCTS
async function loadProducts() {
  try {
    const res = await fetch("http://localhost:18080/products");
    const data = await res.json();
    displayProducts(data);
  } catch {
    // fallback data
    const data = [
      {id:1,name:"Shoes",price:999,img:"https://via.placeholder.com/200"},
      {id:2,name:"Watch",price:1999,img:"https://via.placeholder.com/200"},
      {id:3,name:"Phone",price:14999,img:"https://via.placeholder.com/200"}
    ];
    displayProducts(data);
  }
}

// DISPLAY PRODUCTS
function displayProducts(products) {
  const container = document.getElementById("products");
  container.innerHTML = "";

  products.forEach(p => {
    const card = document.createElement("div");
    card.className = "card";

    card.innerHTML = `
      <img src="${p.img}">
      <h3>${p.name}</h3>
      <p class="price">₹${p.price}</p>
      <button>Add to Cart</button>
    `;

    card.querySelector("button").addEventListener("click", () => addToCart(p));

    container.appendChild(card);
  });
}

// ADD TO CART
function addToCart(product) {
  cart.push(product);
  document.getElementById("cartCount").innerText = cart.length;
}

// RENDER CART
function renderCart() {
  const container = document.getElementById("cartItems");
  const totalEl = document.getElementById("total");

  container.innerHTML = "";
  let total = 0;

  cart.forEach((p, index) => {
    total += p.price;

    const item = document.createElement("p");
    item.innerHTML = `
      ${p.name} - ₹${p.price}
      <button onclick="removeItem(${index})">Remove</button>
    `;

    container.appendChild(item);
  });

  totalEl.innerText = "Total: ₹" + total;
}

// REMOVE ITEM
window.removeItem = function(index) {
  cart.splice(index, 1);
  document.getElementById("cartCount").innerText = cart.length;
  renderCart();
}

// CHECKOUT
window.checkout = function() {
  alert("Order placed!");
  cart = [];
  document.getElementById("cartCount").innerText = 0;
  showPage('home');
}

// SEARCH
document.getElementById("search").addEventListener("input", function() {
  const value = this.value.toLowerCase();

  document.querySelectorAll('.card').forEach(card => {
    const name = card.querySelector('h3').innerText.toLowerCase();
    card.style.display = name.includes(value) ? 'block' : 'none';
  });
});

// INIT
loadProducts();

});
